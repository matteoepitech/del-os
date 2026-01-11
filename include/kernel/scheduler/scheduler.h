/*
** DELOS PROJECT, 2026
** include/kernel/scheduler/scheduler
** File description:
** Scheduler header file
*/

#include <kernel/scheduler/task.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_SCHEDULER_H_
    #define KERNEL_SCHEDULER_H_

/* @brief This variable is the head/tail of the task */
extern task_t *ktask_head;
extern task_t *ktask_tail;
/* @brief This variable contains the current task executed on the CPU */
extern task_t *ktask_current;

/**
 * @brief Initialize the scheduler stuff.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_init(void);

/**
 * @brief Add a task in the scheduler.
 *
 * @param task   The task to add on the scheduler nodes
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_add_task(task_t *task);

/**
 * @brief Get the current task in the list of the scheduler.
 *
 * @return The current task if there is one.
 */
task_t *
kscheduler_get_current_task(void);

/**
 * @brief Run the current task one time.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_run_once(void);

/**
 * @brief Go to the next task planned by the scheduler.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kscheduler_go_next(void);

#endif /* ifndef KERNEL_SCHEDULER_H_ */
