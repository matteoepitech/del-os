/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_log_nb
** File description:
** SYSCALL log_nb source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/sys/syscall.h>
#include <utils/misc/print.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry log_nb.
 *
 * @param a1    The number
 * @param a2    Unused
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_log_nb(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    KPRINTF_DEBUG("Number: %d", a1);
    return 0;
}
