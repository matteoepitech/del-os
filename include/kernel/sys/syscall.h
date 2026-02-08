/*
** DELOS PROJECT, 2026
** include/kernel/sys/syscall
** File description:
** SYSCALL header file
*/

#include <kernel/fs/vfs/vfs_stat.h>
#include <types.h>

#ifndef KERNEL_SYS_SYSCALL_H_
    #define KERNEL_SYS_SYSCALL_H_

    #ifndef KERNEL_SYSCALL_LIST
        #define KERNEL_SYSCALL_LIST

        #define KSYS_EXIT   0
        #define KSYS_OPEN   1
        #define KSYS_CLOSE  2
        #define KSYS_CHDIR  3
        #define KSYS_MKDIR  4
        #define KSYS_RMDIR  5
        #define KSYS_STAT   6
        #define KSYS_FSTAT  7
        #define KSYS_UNLINK 8
        #define KSYS_WRITE  9
    #endif /* ifndef KERNEL_SYSCALL_LIST */

/* @brief Syscall function typedef */
typedef int32_t (*syscall_fn_t)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);

/**
 * @brief Doing a syscall using the interruption 0x80.
 *
 * @param eax    Register..
 * @param ebx    Register..
 * @param ecx    Register..
 * @param edx    Register..
 * @param esi    Register..
 *
 * @return The return of the syscall.
 */
int32_t
__syscall__(int32_t eax, int32_t ebx, int32_t ecx, int32_t edx, int32_t esi, int32_t edi);

/*
 * @brief Array of syscalls. The position of this array determine the ID of the syscall in the EAX register
 */
extern const syscall_fn_t syscalls_array[];

/* Syscall entry points (register-based, six arguments) in order. */
int32_t ksys_exit(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_open(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_close(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_chdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_mkdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_rmdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_stat(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_fstat(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_unlink(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_write(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);

#endif /* ifndef KERNEL_SYS_SYSCALL_H_ */
