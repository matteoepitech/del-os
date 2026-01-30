/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/process/process_manager
** File description:
** Process manager source file
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/memory/pmm/pmm.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/misc/panic.h>
#include <utils/misc/print.h>
#include <kernel/sys/gdt.h>
#include <utils/asm/hlt.h>

/* @brief Current PID variable to keep track of the PID */
pid_t kprocess_pid_current = 0;

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
    task->_process->_ppid = ktask_current->_process->_ppid;
    task->_process->_state = KPROCESS_ALIVE;
    if (kprocess_create_user(process, entry) == KO_FALSE) {
        kfree(process);
        return NULL;
    }
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
    task->_process->_ppid = ktask_current ? ktask_current->_process->_ppid : 0;
    task->_process->_state = KPROCESS_ALIVE;
    task->_process->_page_directory = kvmm_page_directory;
    task->_process->_page_directory_phys = VIRT_TO_PHYS(kvmm_page_directory);
    return process;
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
    kernel_process->_page_directory = kvmm_page_directory;
    kernel_process->_page_directory_phys = VIRT_TO_PHYS(kvmm_page_directory);
    return kernel_process;
}

/**
 * @brief Create an userspace for a process.
 *
 * @param process    The process to create an userspace
 * @param entry      The entry function of the main thread
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kprocess_create_user(process_t *process, void (*entry)(void))
{
    page_directory_t *kernel_pd = KVMM_GET_PD;
    page_directory_t *new_pd = NULL;
    paddr_t pd_phys = 0;
    paddr_t stack_phys = 0;
    vaddr_t vaddr = KPROCESS_USER_STACK_BOTTOM;

    if (process == NULL || process->_main_thread == NULL) {
        return KO_FALSE;
    }
    pd_phys = (paddr_t) kpmm_alloc_pages(1);
    if (pd_phys == 0) {
        return KO_FALSE;
    }
    kvmm_unmap_page(KPROCESS_PD_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_PD_TMP_VADDR, pd_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    new_pd = (page_directory_t *) KPROCESS_PD_TMP_VADDR;
    kmemset(new_pd, 0, sizeof(page_directory_t));
    for (uint32_t i = KVMM_PD_INDEX; i < 1023; i++) {
        new_pd->_entries[i] = kernel_pd->_entries[i];
    }
    new_pd->_entries[1023]._present = 1;
    new_pd->_entries[1023]._rw = 1;
    new_pd->_entries[1023]._user = 0;
    new_pd->_entries[1023]._table_addr = pd_phys >> 12;
    stack_phys = (paddr_t) kpmm_alloc_pages(KPROCESS_USER_STACK_PAGE_COUNT);
    if (stack_phys == 0) {
        return KO_FALSE;
    }
    for (uint32_t i = 0; i < KPROCESS_USER_STACK_PAGE_COUNT; i++, vaddr += KPMM_PAGE_SIZE) {
        uint32_t pde_index = KVMM_GET_PDE_INDEX(vaddr);
        uint32_t pte_index = KVMM_GET_PTE_INDEX(vaddr);
        paddr_t pt_phys = 0;

        if (new_pd->_entries[pde_index]._present == KO_FALSE) {
            pt_phys = (paddr_t) kpmm_alloc_pages(1);
            if (pt_phys == 0) {
                return KO_FALSE;
            }
            kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
            if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
                return KO_FALSE;
            }
            kmemset((void *) KPROCESS_PT_TMP_VADDR, 0, sizeof(page_table_t));
            new_pd->_entries[pde_index]._present = 1;
            new_pd->_entries[pde_index]._rw = 1;
            new_pd->_entries[pde_index]._user = 1;
            new_pd->_entries[pde_index]._table_addr = pt_phys >> 12;
        }
        pt_phys = new_pd->_entries[pde_index]._table_addr << 12;
        kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
        if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
            return KO_FALSE;
        }
        page_table_t *pt = (page_table_t *) KPROCESS_PT_TMP_VADDR;
        if (pt->_entries[pte_index]._present == OK_TRUE) {
            return KO_FALSE;
        }
        pt->_entries[pte_index]._present = 1;
        pt->_entries[pte_index]._rw = 1;
        pt->_entries[pte_index]._user = 1;
        pt->_entries[pte_index]._frame = (stack_phys + (i * KPMM_PAGE_SIZE)) >> 12;
    }
    paddr_t last_page_phys = stack_phys + ((KPROCESS_USER_STACK_PAGE_COUNT - 1) * KPMM_PAGE_SIZE);
    kvmm_unmap_page(KPROCESS_STACK_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_STACK_TMP_VADDR, last_page_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    uint32_t *stack_end = (uint32_t *) (KPROCESS_STACK_TMP_VADDR + KPMM_PAGE_SIZE);
    stack_end[-1] = (uint32_t) entry;
    stack_end[-2] = 0;
    process->_main_thread->_ctx._esp = KPROCESS_USER_STACK_TOP - 8;
    process->_page_directory = (page_directory_t *) pd_phys;
    process->_page_directory_phys = pd_phys;
    process->_main_thread->_ctx._cs = KGDT_USER_CODE_SELECTOR;
    process->_main_thread->_ctx._ds = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._es = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._fs = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._gs = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._esp = KPROCESS_USER_STACK_TOP - 8;
    return OK_TRUE;
}
