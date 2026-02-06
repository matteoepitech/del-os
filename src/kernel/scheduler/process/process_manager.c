/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/process/process_manager
** File description:
** Process manager source file
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/memory/pmm/pmm.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/misc/panic.h>
#include <utils/misc/print.h>
#include <kernel/fs/fd/fd.h>
#include <kernel/sys/gdt.h>
#include <utils/asm/hlt.h>
#include <defines.h>

/* @brief Current PID variable to keep track of the PID */
pid_t kprocess_pid_current = 0;

/**
 * @brief Kernel main task entry.
 */
static void
ktask_kernel_entry(void)
{
    while (OK_TRUE);
    return;
}

/**
 * @brief Attach file descriptors to a process. Such as stdin stdout stderr.
 *
 * @param process    The process to attach the file descriptors
 */
static void
kprocess_attach_std_streams(process_t *process)
{
    if (process == NULL) {
        return;
    }
    if (kfd_open_for_process(process, "/dev/stdin", KVFS_O_RDONLY, 0) == KFD_ERROR) {
        KPRINTF_ERROR("process_manager: failed to open /dev/stdin for a process while creation");
        return;
    }
    if (kfd_open_for_process(process, "/dev/stdout", KVFS_O_WRONLY, 0) == KFD_ERROR) {
        KPRINTF_ERROR("process_manager: failed to open /dev/stdout for a process while creation");
        return;
    }
    if (kfd_open_for_process(process, "/dev/stderr", KVFS_O_WRONLY, 0) == KFD_ERROR) {
        KPRINTF_ERROR("process_manager: failed to open /dev/stderr for a process while creation");
        return;
    }
}

/**
 * @brief The process to create.
 *
 * @param void (*entry)(void)    The entry of the main thread.
 *
 * @return The process pointer.
 */
process_t *
kprocess_create(void (*entry)(void))
{
    process_t *process = NULL;
    task_t *task = NULL;

    if (entry == NULL) {
        return NULL;
    }
    process = kcalloc(sizeof(process_t));
    if (process == NULL) {
        return NULL;
    }
    task = ktask_create(entry);
    if (task == NULL) {
        return NULL;
    }
    task->_process = process;
    task->_process->_main_thread = task;
    task->_process->_pid = kprocess_pid_current++;
    task->_process->_ppid = ktask_current->_process->_pid;
    task->_process->_state = KPROCESS_ALIVE;
    if (kprocess_create_user(process, entry) == KO_FALSE) {
        kfree(process);
        return NULL;
    }
    kprocess_attach_std_streams(process);
    return process;
}

/**
 * @brief Create a kernel-only process sharing the kernel address space.
 *
 * @param entry    Entry point
 *
 * @return The process pointer.
 */
process_t *
kprocess_create_kernel(void (*entry)(void))
{
    process_t *process = NULL;
    task_t *task = NULL;

    if (entry == NULL) {
        return NULL;
    }
    process = kcalloc(sizeof(process_t));
    if (process == NULL) {
        return NULL;
    }
    task = ktask_create(entry);
    if (task == NULL) {
        return NULL;
    }
    task->_process = process;
    task->_process->_main_thread = task;
    task->_process->_pid = kprocess_pid_current++;
    task->_process->_ppid = ktask_current ? ktask_current->_process->_pid : 0;
    task->_process->_state = KPROCESS_ALIVE;
    task->_process->_page_directory = kvmm_page_directory;
    task->_process->_page_directory_phys = VIRT_TO_PHYS(kvmm_page_directory);
    kprocess_attach_std_streams(process);
    return process;
}

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @return The process of the kernel.
 */
process_t *
kprocess_kernel_init(void)
{
    process_t *kernel_process = kcalloc(sizeof(process_t));

    if (kernel_process == NULL) {
        KPANIC("Kernel process cannot be initialized.");
        return NULL;
    }
    kernel_process->_pid = 0;
    kernel_process->_ppid = 0;
    kernel_process->_state = KPROCESS_ALIVE;
    kernel_process->_main_thread = ktask_create(ktask_kernel_entry);
    kernel_process->_main_thread->_process = kernel_process;
    kernel_process->_page_directory = kvmm_page_directory;
    kernel_process->_page_directory_phys = VIRT_TO_PHYS(kvmm_page_directory);
    kprocess_set_name("ksystem", kernel_process);
    kprocess_pid_current = 1;
    kprocess_attach_std_streams(kernel_process);
    return kernel_process;
}
