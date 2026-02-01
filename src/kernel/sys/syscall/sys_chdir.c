/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_chdir
** File description:
** SYSCALL chdir source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry chdir.
 *
 * @param a1    Path pointer (const char *)
 * @param a2    Unused
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 * @param a6    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_chdir(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5, UNUSED sysarg_t a6)
{
    return kvfs_chdir((const char *) a1);
}
