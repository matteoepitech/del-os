/*
** DELOS PROJECT, 2026
** include/kernel/scheduler/process
** File description:
** Process header file
*/

#include <kernel/user/user_space.h>
#include <kernel/scheduler/task.h>
#include <kernel/memory/vmm/vmm.h>
#include <kernel/fs/fd/fd.h>
#include <types.h>

#ifndef KERNEL_SCHEDULER_PROCESS_H_
    #define KERNEL_SCHEDULER_PROCESS_H_

    #define KERNEL_PROCESS_STACK_SIZE 2048
    #define KERNEL_PROCESS_FD_MAX_AMOUNT KFD_MAX_COUNT

    #define KPROCESS_PD_TMP_VADDR 0xE0000000
    #define KPROCESS_PT_TMP_VADDR 0xE0001000
    #define KPROCESS_STACK_TMP_VADDR 0xE0002000

    #define KERNEL_PROCESS_NAME_MAX_LEN 32

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
    char _name[KERNEL_PROCESS_NAME_MAX_LEN];
    file_desc_t *_fds[KERNEL_PROCESS_FD_MAX_AMOUNT];
    page_directory_t *_page_directory;
    paddr_t _page_directory_phys;
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
 * @brief Create a kernel-only process (ring0, shared kernel address space).
 *
 * @param entry   The entry of the main thread.
 *
 * @return The process pointer.
 */
process_t *
kprocess_create_kernel(void (*entry)(void));

/**
 * @brief Create an userspace for a process.
 *
 * @param process    The process to create an userspace
 * @param entry      The entry function of the main thread (for initial user stack frame)
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kprocess_create_user(process_t *process, void (*entry)(void));

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @return The process of the kernel.
 */
process_t *
kprocess_kernel_init(void);

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @param name          The name of the process.
 */
void
kprocess_set_name(char *name, process_t *process);

#endif /* ifndef KERNEL_SCHEDULER_PROCESS_H_ */
