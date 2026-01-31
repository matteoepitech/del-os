/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/process/process_user
** File description:
** Userspace process setup helpers
*/

#include <kernel/scheduler/process.h>
#include <kernel/scheduler/task.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/memory/pmm/pmm.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/sys/gdt.h>
#include <defines.h>

/**
 * @brief Allocate a fresh page directory and map it temporarily.
 *
 * @param out_pd       Returned mapped PD (temporary kernel VA)
 * @param out_pd_phys  Returned physical address of PD
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
alloc_page_directory(page_directory_t **out_pd, paddr_t *out_pd_phys)
{
    paddr_t pd_phys = (paddr_t) kpmm_alloc_pages(1);
    page_directory_t *pd = NULL;

    if (pd_phys == 0) {
        return KO_FALSE;
    }
    kvmm_unmap_page(KPROCESS_PD_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_PD_TMP_VADDR, pd_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    pd = (page_directory_t *) KPROCESS_PD_TMP_VADDR;
    kmemset(pd, 0, sizeof(page_directory_t));
    *out_pd = pd;
    *out_pd_phys = pd_phys;
    return OK_TRUE;
}

/**
 * @brief Clone kernel-space mappings into a new page directory.
 *
 * Copies all PDEs covering the higher-half kernel region from an existing
 * directory and installs a self-referencing entry at index 1023 so the new PD
 * can be accessed through its own virtual address space.
 *
 * @param dst      Destination page directory (already mapped in kernel space).
 * @param src      Source kernel page directory to copy from.
 * @param pd_phys  Physical address of the destination PD (for self-reference).
 */
static void
clone_kernel_high_half(page_directory_t *dst, page_directory_t *src, paddr_t pd_phys)
{
    for (uint32_t i = KVMM_PD_INDEX; i < 1023; i++) {
        dst->_entries[i] = src->_entries[i];
    }
    dst->_entries[1023]._present = 1;
    dst->_entries[1023]._rw = 1;
    dst->_entries[1023]._user = 0;
    dst->_entries[1023]._table_addr = pd_phys >> 12;
}

/**
 * @brief Map the scheduler stub page so user mode can invoke it.
 *
 * Clones the page table containing `ktask_stubs` into a user-accessible copy
 * and updates the provided PD entry to point at it, enabling ring3 to jump to
 * the stub code during context switches.
 *
 * @param pd                       Page directory being constructed.
 * @param stub_user_pt_phys_out    Returns physical address of the cloned PT.
 * @param stub_pde_out             Returns PDE index used for the stub mapping.
 *
 * @return OK_TRUE on success, KO_FALSE on allocation/mapping failures.
 */
static bool32_t
map_stub_user_page(page_directory_t *pd, paddr_t *stub_user_pt_phys_out, uint32_t *stub_pde_out)
{
    vaddr_t stub_vaddr = (vaddr_t) ktask_stubs;
    uint32_t stub_pde = KVMM_GET_PDE_INDEX(stub_vaddr);
    uint32_t stub_pte = KVMM_GET_PTE_INDEX(stub_vaddr);
    page_table_t *kernel_pt = KVMM_GET_PT_VIA_PDE(stub_pde);
    page_table_t *user_pt = NULL;
    paddr_t user_pt_phys;

    if (kernel_pt == NULL || kernel_pt->_entries[stub_pte]._present == KO_FALSE) {
        return KO_FALSE;
    }
    user_pt_phys = (paddr_t) kpmm_alloc_pages(1);
    if (user_pt_phys == 0) {
        return KO_FALSE;
    }
    kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, user_pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    user_pt = (page_table_t *) KPROCESS_PT_TMP_VADDR;
    kmemcpy(user_pt, kernel_pt, sizeof(page_table_t));
    user_pt->_entries[stub_pte]._user = 1; /* allow fetch in ring3 */
    pd->_entries[stub_pde]._present = 1;
    pd->_entries[stub_pde]._rw = 1;
    pd->_entries[stub_pde]._user = 1;
    pd->_entries[stub_pde]._table_addr = user_pt_phys >> 12;
    *stub_user_pt_phys_out = user_pt_phys;
    *stub_pde_out = stub_pde;
    return OK_TRUE;
}

/**
 * @brief Map the entry point if it lives in kernel space.
 *
 * If the provided entry address is in the kernel higher half, duplicate its
 * page table into user space (reusing the stub PT when possible) and mark the
 * entry page user-accessible so ring3 can jump into it.
 *
 * @param pd                 Destination page directory to modify.
 * @param entry              Function pointer representing the user entry point.
 * @param stub_user_pt_phys  Physical address of the stub user page table.
 * @param stub_pde           PDE index that already maps the stub page.
 *
 * @return OK_TRUE on success, KO_FALSE if mapping failed or memory is missing.
 */
static bool32_t
map_entry_if_kernel(page_directory_t *pd, void (*entry)(void), paddr_t stub_user_pt_phys, uint32_t stub_pde)
{
    vaddr_t entry_vaddr = (vaddr_t) entry;
    uint32_t entry_pde = KVMM_GET_PDE_INDEX(entry_vaddr);
    uint32_t entry_pte = KVMM_GET_PTE_INDEX(entry_vaddr);
    page_table_t *kernel_pt = KVMM_GET_PT_VIA_PDE(entry_pde);
    page_table_t *user_pt = NULL;
    paddr_t user_pt_phys = 0;

    if (entry_vaddr < KERNEL_VIRTUAL_BASE) {
        return OK_TRUE;
    }
    if (kernel_pt == NULL || kernel_pt->_entries[entry_pte]._present == KO_FALSE) {
        return KO_FALSE;
    }
    if (entry_pde == stub_pde) {
        user_pt_phys = stub_user_pt_phys;
    } else {
        user_pt_phys = (paddr_t) kpmm_alloc_pages(1);
        if (user_pt_phys == 0) {
            return KO_FALSE;
        }
        kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
        if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, user_pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
            return KO_FALSE;
        }
        user_pt = (page_table_t *) KPROCESS_PT_TMP_VADDR;
        kmemcpy(user_pt, kernel_pt, sizeof(page_table_t));
        pd->_entries[entry_pde]._present = 1;
        pd->_entries[entry_pde]._rw = 1;
        pd->_entries[entry_pde]._user = 1;
        pd->_entries[entry_pde]._table_addr = user_pt_phys >> 12;
    }
    kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, user_pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    user_pt = (page_table_t *) KPROCESS_PT_TMP_VADDR;
    user_pt->_entries[entry_pte]._user = 1;
    return OK_TRUE;
}

/**
 * @brief Allocate and map the initial userspace stack.
 *
 * Reserves KPROCESS_USER_STACK_PAGE_COUNT physical pages and maps them into
 * the process page directory with user permissions starting at
 * KPROCESS_USER_STACK_BOTTOM.
 *
 * @param pd              Page directory to receive the stack mappings.
 * @param out_stack_phys  Returns physical base address of the allocated stack.
 *
 * @return OK_TRUE on success, KO_FALSE on allocation or mapping failure.
 */
static bool32_t
map_user_stack(page_directory_t *pd, paddr_t *out_stack_phys)
{
    paddr_t stack_phys = (paddr_t) kpmm_alloc_pages(KPROCESS_USER_STACK_PAGE_COUNT);
    vaddr_t vaddr = KPROCESS_USER_STACK_BOTTOM;
    page_table_t *pt = NULL;

    if (stack_phys == 0) {
        return KO_FALSE;
    }
    for (uint32_t i = 0; i < KPROCESS_USER_STACK_PAGE_COUNT; i++, vaddr += KPMM_PAGE_SIZE) {
        uint32_t pde_index = KVMM_GET_PDE_INDEX(vaddr);
        uint32_t pte_index = KVMM_GET_PTE_INDEX(vaddr);
        paddr_t pt_phys = 0;

        if (pd->_entries[pde_index]._present == KO_FALSE) {
            pt_phys = (paddr_t) kpmm_alloc_pages(1);
            if (pt_phys == 0) {
                return KO_FALSE;
            }
            kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
            if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
                return KO_FALSE;
            }
            kmemset((void *) KPROCESS_PT_TMP_VADDR, 0, sizeof(page_table_t));
            pd->_entries[pde_index]._present = 1;
            pd->_entries[pde_index]._rw = 1;
            pd->_entries[pde_index]._user = 1;
            pd->_entries[pde_index]._table_addr = pt_phys >> 12;
        }
        pt_phys = pd->_entries[pde_index]._table_addr << 12;
        kvmm_unmap_page(KPROCESS_PT_TMP_VADDR);
        if (kvmm_map_page(KPROCESS_PT_TMP_VADDR, pt_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
            return KO_FALSE;
        }
        pt = (page_table_t *) KPROCESS_PT_TMP_VADDR;
        if (pt->_entries[pte_index]._present == OK_TRUE) {
            return KO_FALSE;
        }
        pt->_entries[pte_index]._present = 1;
        pt->_entries[pte_index]._rw = 1;
        pt->_entries[pte_index]._user = 1;
        pt->_entries[pte_index]._frame = (stack_phys + (i * KPMM_PAGE_SIZE)) >> 12;
    }
    *out_stack_phys = stack_phys;
    return OK_TRUE;
}

/**
 * @brief Seed the user stack with the initial context.
 *
 * Maps the last stack page temporarily, writes the entry return address and
 * clears the first argument slot, then updates the main thread ESP to point to
 * the top of userspace stack.
 *
 * @param stack_phys  Physical base address of the allocated user stack.
 * @param entry       Entry point to push as the initial return address.
 * @param process     Target process whose main thread context is updated.
 *
 * @return OK_TRUE on success, KO_FALSE if mapping fails.
 */
static bool32_t
setup_initial_stack(paddr_t stack_phys, void (*entry)(void), process_t *process)
{
    paddr_t last_page_phys = stack_phys + ((KPROCESS_USER_STACK_PAGE_COUNT - 1) * KPMM_PAGE_SIZE);
    uint32_t *stack_end = NULL;

    kvmm_unmap_page(KPROCESS_STACK_TMP_VADDR);
    if (kvmm_map_page(KPROCESS_STACK_TMP_VADDR, last_page_phys, KVMM_FLAG_PRESENT | KVMM_FLAG_RW | KVMM_FLAG_KERNEL) == KO_FALSE) {
        return KO_FALSE;
    }
    stack_end = (uint32_t *) (KPROCESS_STACK_TMP_VADDR + KPMM_PAGE_SIZE);
    stack_end[-1] = (uint32_t) entry;
    stack_end[-2] = 0;
    process->_main_thread->_ctx._esp = KPROCESS_USER_STACK_TOP - 8;
    return OK_TRUE;
}

/**
 * @brief Finalize CPU segment and PD pointers for the new process.
 *
 * Writes the page directory pointers into the process structure and prepares
 * the main thread segment selectors and ESP to start in user mode.
 *
 * @param process  Process to finalize.
 * @param pd_phys  Physical address of the process page directory.
 */
static void
finalize_process_context(process_t *process, paddr_t pd_phys)
{
    process->_page_directory = (page_directory_t *) pd_phys;
    process->_page_directory_phys = pd_phys;
    process->_main_thread->_ctx._cs = KGDT_USER_CODE_SELECTOR;
    process->_main_thread->_ctx._ds = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._es = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._fs = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._gs = KGDT_USER_DATA_SELECTOR;
    process->_main_thread->_ctx._esp = KPROCESS_USER_STACK_TOP - 8;
}

/**
 * @brief Build a runnable userspace process around a given entry point.
 *
 * Allocates a new page directory, clones kernel mappings, sets up stub and
 * entry mappings, allocates the user stack, seeds the initial stack frame, and
 * finalizes the process context ready to run in ring3.
 *
 * @param process  Process structure to populate (expects a main thread).
 * @param entry    Function pointer to the process entry point.
 *
 * @return OK_TRUE on success, KO_FALSE on any allocation or mapping error.
 */
bool32_t kprocess_create_user(process_t *process, void (*entry)(void))
{
    page_directory_t *kernel_pd = KVMM_GET_PD;
    page_directory_t *new_pd = NULL;
    paddr_t pd_phys = 0;
    paddr_t stack_phys = 0;
    paddr_t stub_user_pt_phys = 0;
    uint32_t stub_pde = 0;

    if (process == NULL || process->_main_thread == NULL || entry == NULL) {
        return KO_FALSE;
    }
    if (alloc_page_directory(&new_pd, &pd_phys) == KO_FALSE) {
        return KO_FALSE;
    }
    clone_kernel_high_half(new_pd, kernel_pd, pd_phys);
    if (map_stub_user_page(new_pd, &stub_user_pt_phys, &stub_pde) == KO_FALSE) {
        return KO_FALSE;
    }
    if (map_entry_if_kernel(new_pd, entry, stub_user_pt_phys, stub_pde) == KO_FALSE) {
        return KO_FALSE;
    }
    if (map_user_stack(new_pd, &stack_phys) == KO_FALSE) {
        return KO_FALSE;
    }
    if (setup_initial_stack(stack_phys, entry, process) == KO_FALSE) {
        return KO_FALSE;
    }
    finalize_process_context(process, pd_phys);
    return OK_TRUE;
}
