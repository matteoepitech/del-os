/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/sys_write
** File description:
** SYSCALL write source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/vfs/vfs.h>
#include <defines.h>

/**
 * @brief Syscall entry write.
 *
 * @param a1    File descriptor
 * @param a2    Buffer to write in
 * @param a3    Size of the buffer
 * @param a4    Unused
 * @param a5    Unused
 *
 * @return 0 on success, -1 on error.
 */
int32_t
ksys_write(sysarg_t a1, sysarg_t a2, sysarg_t a3, UNUSED sysarg_t a4, UNUSED sysarg_t a5)
{
    return kfd_write(a1, (const char *) a2, a3);
}
