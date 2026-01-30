/*
** DELOS PROJECT, 2026
** include/kernel/scheduler/task
** File description:
** Scheduler task header file
*/

#include <kernel/interruption/isr.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_SCHEDULER_TASK_H_
    #define KERNEL_SCHEDULER_TASK_H_

/*
 * @brief Enumeration for the differents types of a task's state.
 *        - IDLE    = Task is doing nothing
 *        - RUNNING = Task is running and executing some stuff on the CPU
 *        - PAUSED  = Task is paused of execution and waits to be run
 *        - ZOMBIE  = Task is zombie..
 */
typedef enum {
    KTASK_IDLE,
    KTASK_RUNNING,
    KTASK_PAUSED,
    KTASK_ZOMBIE,
} task_state_t;

struct process_s;
typedef struct process_s process_t;

/*
 * @brief Structure for a task. A task is basically a process/thread. Something runnable.
 *        Contains some informations for the context switching.
 */
typedef struct task_s {
    isr_registers_t _ctx;
    void *_stack_ptr;
    size_t _stack_size;
    task_state_t _state;
    struct task_s *_next;
    process_t *_process;
} task_t;

/**
 * @brief Create a new task and return it.
 *
 * @param void (*entry)(void)    A function pointer to the handler of the task
 *
 * @return The new task created.
 */
task_t *
ktask_create(void (*entry)(void));

/**
 * @brief Remove the current task from the list of tasks.
 */
void
ktask_exit(void);

/**
 * @brief Yield the task and wait while doing an hlt for the CPU. DO NOT HARD HALT HERE.
 */
void
ktask_yield(void);

/**
 * @brief ONLY use this function to create an entry.
 *        Doing your entry and if your entry is returning, doing an ktask_exit().
 *
 * @param *(entry)(void)     The entry of the function
 */
__attribute__((noreturn)) void
ktask_stubs(void (*entry(void)));

#endif /* ifndef KERNEL_SCHEDULER_TASK_H_ */
