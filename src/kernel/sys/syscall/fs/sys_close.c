/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/fs/sys_close
** File description:
** SYSCALL close source file
*/

#include <kernel/fs/fd/fd.h>
#include <kernel/sys/syscall.h>
#include <defines.h>

/**
 * @brief Syscall entry close.
 *
 * @param a1    File descriptor to close
 * @param a2    Unused
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_close(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kfd_close((fd_t) a1);
}
