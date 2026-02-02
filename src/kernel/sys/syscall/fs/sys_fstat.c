/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/fs/sys_fstat
** File description:
** SYSCALL fstat source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry fstat.
 *
 * @param a1    File descriptor to stat
 * @param a2    Pointer to vfs_stat_t buffer
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_fstat(sysarg_t a1, sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kfd_stat((fd_t) a1, (vfs_stat_t *) a2);
}
