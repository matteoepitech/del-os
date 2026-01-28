/*
** DELOS PROJECT, 2025
** src/kernel/kernel
** File description:
** Kernel Source file main
*/

#include <kernel/memory/early_allocator/early_alloc.h>
#include <kernel/interruption/interruption.h>
#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/interruption/idt.h>
#include <kernel/interruption/pic.h>
#include <kernel/memory/pmm/pmm.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/memory/stack.h>
#include <kernel/shell/shell.h>
#include <kernel/misc/panic.h>
#include <utils/misc/print.h>
#include <utils/asm/hlt.h>
#include <kernel/fs/fs.h>
#include <defines.h>

/**
 * @brief Kernel main entry point.
 *        This function is located at 0x10000 in the memory.
 *        This function is called by the kernel_entry.s source file.
 */
void
kmain(void)
{
    kinterruption_extern_stop();
    kpic_remap();
    kidt_create_ptr(&idt_ptr);
    kidt_load_cpu(&idt_ptr);
    kpit_timer_init(PIT_TARGET_FREQUENCY);
    ktty_cursor_set_visibility(OK_TRUE);
    kinterruption_extern_start();
    kearly_malloc_disable();
    kmalloc_init();
    kfs_init();
    kscheduler_init();

    kscheduler_add_task(ktask_create(kshell_start_task));

    /* Should never goes here for the moment */
    KHLT_HARD_DO();
    return;
}
