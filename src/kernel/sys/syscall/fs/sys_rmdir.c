/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/fs/sys_rmdir
** File description:
** SYSCALL rmdir source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fs_utils.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry rmdir.
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
ksys_rmdir(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return krmdir((const char *) a1);
}
