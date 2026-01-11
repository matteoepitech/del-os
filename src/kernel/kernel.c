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

static void test_task_idle(void)
{
    KPRINTF_OK("PRINT FROM IDLE");
    return;
}

static void test_task_idle2(void)
{
    KPRINTF_OK("PRINT FROM IDLE 2");
    return;
}

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
    
    task_t *t1 = ktask_create(test_task_idle);
    task_t *t2 = ktask_create(test_task_idle2);

    if (kscheduler_add_task(t1) == OK_TRUE)
        KPRINTF_OK("Task 1 added.");
    if (kscheduler_add_task(t2) == OK_TRUE)
        KPRINTF_OK("Task 2 added.");

    kscheduler_run_once();
    kscheduler_run_once();

    kshell_start();

    /* Should never goes here for the moment */
    KHLT_HARD_DO();
    return;
}
