/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/scheduler
** File description:
** Scheduler source file main
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>

/* @brief This variable is the head/tail of the task */
task_t *ktask_head = NULL;
task_t *ktask_tail = NULL;
/* @brief This variable contains the current task executed on the CPU */
task_t *ktask_current = NULL;

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
        ktask_current = task;
    } else {
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
 * @brief Run the current task one time.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_run_once(void)
{
    if (ktask_current == NULL) {
        return KO_FALSE;
    }
    ktask_current->_state = KTASK_RUNNING;
    ktask_current->_entry();
    kscheduler_go_next();
    return OK_TRUE;
}

/**
 * @brief Go to the next task planned by the scheduler.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_go_next(void)
{
    if (ktask_current->_next == NULL) {
        ktask_current = ktask_head;
        return OK_TRUE;
    } else {
        ktask_current = ktask_current->_next;
        return OK_TRUE;
    }
    return KO_FALSE;
}
