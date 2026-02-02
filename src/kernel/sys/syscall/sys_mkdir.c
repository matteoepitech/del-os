/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_mkdir
** File description:
** SYSCALL mkdir source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fs_utils.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry mkdir.
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
ksys_mkdir(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kmkdir((const char *) a1);
}
