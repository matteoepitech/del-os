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

/* @brief Syscall function typedef */
typedef int32_t (*syscall_fn_t)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);

/* Syscall entry points (register-based, six arguments) */
int32_t ksys_chdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_open(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_close(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_fstat(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_stat(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_mkdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_unlink(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
int32_t ksys_rmdir(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);

#endif /* ifndef KERNEL_SYS_SYSCALL_H_ */
