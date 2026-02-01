/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_exit
** File description:
** SYSCALL exit source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/scheduler/task.h>
#include <kernel/sys/syscall.h>
#include <utils/misc/print.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Syscall entry exit.
 *
 * @param a1    Unused
 * @param a2    Unused
 * @param a3    Unused
 * @param a4    Unused
 * @param a5    Unused
 * @param a6    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_exit(UNUSED sysarg_t a1, UNUSED sysarg_t a2, UNUSED sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5, UNUSED sysarg_t a6)
{
    KPRINTF_DEBUG("Exiting process");
    ktask_exit();
    return 0;
}
