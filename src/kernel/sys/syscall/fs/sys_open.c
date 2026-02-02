/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/fs/sys_open
** File description:
** SYSCALL open source file
*/

#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry open.
 *
 * @param a1    Path pointer (const char *)
 * @param a2    Flags
 * @param a3    Mode (created file permissions)
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return New fd or KFD_ERROR.
 */
int32_t
ksys_open(sysarg_t a1, sysarg_t a2, sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kfd_open((const char *) a1, (int32_t) a2, (mode_t) a3);
}
