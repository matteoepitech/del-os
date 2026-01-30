/*
** DELOS PROJECT, 2026
** include/kernel/scheduler/process
** File description:
** Process header file
*/

#include <kernel/scheduler/task.h>
#include <kernel/fs/fd/fd.h>
#include <types.h>

#ifndef KERNEL_SCHEDULER_PROCESS_H_
    #define KERNEL_SCHEDULER_PROCESS_H_

    #define KERNEL_PROCESS_STACK_SIZE 2048
    #define KERNEL_PROCESS_FD_MAX_AMOUNT 16

/* @brief Current PID variable to keep track of the PID */
extern pid_t kprocess_pid_current;

/*
 * @brief Enumeration for the differents types of a process's state.
 *        - DEAD  = Process is dead
 *        - ALIVE = Process is running and executing some stuff on the CPU
 */
typedef enum {
    KPROCESS_ALIVE,
    KPROCESS_DEAD,
} process_state_t;

/*
 * @brief Structure for a process.
 */
typedef struct process_s {
    pid_t _pid;
    pid_t _ppid;
    task_t *_main_thread;
    process_state_t _state;
    file_desc_t *_fds[KERNEL_PROCESS_FD_MAX_AMOUNT];
} process_t;

/**
 * @brief The process to create.
 *
 * @param void (*entry)(void)    The entry of the main thread.
 *
 * @return The process pointer.
 */
process_t *
kprocess_create(void (*entry)(void));

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @return The task of the kernel.
 */
process_t *
kprocess_kernel_init(void);

#endif /* ifndef KERNEL_SCHEDULER_PROCESS_H_ */
