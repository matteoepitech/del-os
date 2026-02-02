/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_log
** File description:
** SYSCALL log source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/sys/syscall.h>
#include <utils/misc/print.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry log.
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
ksys_log(sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    const char *msg = (const char *) a1;

    if (msg == NULL) {
        return -1;
    }
    KPRINTF_DEBUG("%s", msg);
    return 0;
}
