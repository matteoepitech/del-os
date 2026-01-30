/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/process/process_manager
** File description:
** Process manager source file
*/

#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/misc/panic.h>
#include <utils/misc/print.h>
#include <utils/asm/hlt.h>

/* @brief Current PID variable to keep track of the PID */
pid_t kprocess_pid_current = 0;

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
    task->_process->_ppid = -1;
    task->_process->_state = KPROCESS_ALIVE;
    return process;
}

/**
 * @brief Kernel main task entry.
 */
static void
ktask_kernel_entry(void)
{
    while (OK_TRUE) { }
    return;
}

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @return The task of the kernel.
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
    return kernel_process;
}
