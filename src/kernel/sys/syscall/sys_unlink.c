/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_unlink
** File description:
** SYSCALL unlink source file
*/

#include <kernel/sys/syscall.h>
#include <kernel/fs/vfs/vfs.h>
#include <defines.h>

/**
 * @brief Syscall entry unlink.
 *
 * @param a1    Path pointer (const char *)
 * @param a2    Unused
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_unlink(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kvfs_unlink((const char *) a1);
}
