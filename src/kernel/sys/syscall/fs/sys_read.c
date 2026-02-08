/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/fs/sys_read
** File description:
** SYSCALL read source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/vfs/vfs.h>
#include <defines.h>

/**
 * @brief Syscall entry read.
 *
 * @param a1    File descriptor
 * @param a2    Buffer to read in
 * @param a3    Size of the buffer
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_read(sysarg_t a1, sysarg_t a2, sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kfd_read(a1, (void *) a2, a3);
}
