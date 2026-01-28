/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/scheduler
** File description:
** Scheduler source file main
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/context.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>

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
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_init(void)
{
    ktask_head = NULL;
    ktask_tail = NULL;
    ktask_current = NULL;
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
    } else {
        ktask_tail->_next = task;
        ktask_tail = task;
    }
    if (kscheduler_is_running == KO_FALSE) {
        kscheduler_start();
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
    if (ktask_current == NULL) {
        return NULL;
    }
    if (ktask_current->_next == NULL) {
        return ktask_head;
    }
    return ktask_current->_next;
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
    ktask_current->_ctx._esp = (uint32_t) regs + sizeof(isr_registers_t); // IDK WHY but it's working
    next = kscheduler_pick_next();
    if (next == NULL || next == ktask_current) {
        return KO_FALSE;
    }
    ktask_current = next;
    kcontext_switch_from_isr(&next->_ctx);
    __builtin_unreachable();
}

void
kscheduler_start(void)
{
    if (ktask_head == NULL || kscheduler_is_running == OK_TRUE) {
        return;
    }
    ktask_current = ktask_head;
    ktask_current->_state = KTASK_RUNNING;
    kcontext_enter_first(&ktask_current->_ctx);
    __builtin_unreachable();
}
