/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/scheduler
** File description:
** Scheduler source file main
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/scheduler/context.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/memory/stack.h>
#include <kernel/shell/shell.h>
#include <kernel/memory/mmu.h>
#include <utils/misc/print.h>
#include <kernel/sys/gdt.h>

/* @brief This variable is the head/tail of the task */
task_t *ktask_head = NULL;
task_t *ktask_tail = NULL;
/* @brief This variable contains the current task executed on the CPU */
task_t *ktask_current = NULL;
/* @brief This variable say if the scheduler is running or not */
bool32_t kscheduler_is_running = KO_FALSE;

/**
 * @brief Initialize the scheduler stuff.
 *
 * @param first_task        The first task in the scheduler
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_init(task_t *first_task)
{
    if (kscheduler_is_running == OK_TRUE) {
        return KO_FALSE;
    }
    kscheduler_is_running = KO_FALSE;
    ktask_current = NULL;
    if (ktask_head == NULL) {
        if (first_task == NULL) {
            return KO_FALSE;
        }
        if (kscheduler_add_task(first_task) == KO_FALSE) {
            return KO_FALSE;
        }
    }
    kscheduler_start();
    return OK_TRUE;
}

/**
 * @brief Add a task in the scheduler.
 *
 * @param task   The task to add on the linked list used by the scheduler
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_add_task(task_t *task)
{
    if (task == NULL) {
        return KO_FALSE;
    }
    if (ktask_head == NULL) {
        ktask_head = task;
        ktask_tail = task;
        ktask_tail->_next = ktask_head;
    } else {
        task->_next = ktask_head;
        ktask_tail->_next = task;
        ktask_tail = task;
    }
    return OK_TRUE;
}

/**
 * @brief Get the current task in the list of the scheduler.
 *
 * @return The current task if there is one.
 */
task_t *
kscheduler_get_current_task(void)
{
    return ktask_current;
}

/**
 * @brief Get the next task but not execute it.
 *
 * @return The task.
 */
task_t *
kscheduler_pick_next(void)
{
    task_t *next = NULL;

    if (ktask_current == NULL) {
        return NULL;
    }
    next = ktask_current->_next;
    while (next->_state == KTASK_ZOMBIE && next != ktask_current) {
        next = next->_next;
    }
    if (next->_state == KTASK_ZOMBIE) {
        return NULL;
    }
    return next;
}

/**
 * @brief Do a tick on the scheduler of the kernel.
 */
bool32_t
kscheduler_tick(isr_registers_t *regs)
{
    task_t *next = NULL;

    if (regs == NULL || ktask_current == NULL) {
        return KO_FALSE;
    }
    kmemcpy(&ktask_current->_ctx, regs, sizeof(isr_registers_t));
    if ((regs->_cs & 0x3) == 0x3) {
        ktask_current->_ctx._esp = ((uint32_t *) regs)[17]; // user ESP captured by CPU
        ktask_current->_ctx._ds = KGDT_USER_DATA_SELECTOR;
        ktask_current->_ctx._es = KGDT_USER_DATA_SELECTOR;
        ktask_current->_ctx._fs = KGDT_USER_DATA_SELECTOR;
        ktask_current->_ctx._gs = KGDT_USER_DATA_SELECTOR;
    } else {
        ktask_current->_ctx._esp = (uint32_t) regs + sizeof(isr_registers_t);
    }
    next = kscheduler_pick_next();
    if (next == NULL) {
        return KO_FALSE;
    }
    ktask_current = next;
    if (next->_process != NULL && next->_process->_page_directory_phys != 0) {
        kmmu_load_cr3(next->_process->_page_directory_phys);
        kgdt_set_kernel_stack((uint32_t) &__kernel_stack_end);
    }
    kcontext_switch_from_isr(&next->_ctx);
    __builtin_unreachable();
}

void
kscheduler_start(void)
{
    if (ktask_head == NULL || kscheduler_is_running == OK_TRUE) {
        return;
    }
    kscheduler_is_running = OK_TRUE;
    ktask_current = ktask_head;
    ktask_current->_state = KTASK_RUNNING;
    return;
}
