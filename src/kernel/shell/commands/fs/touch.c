/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/fs/touch
** File description:
** touch command source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Command for touch.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_touch(uint32_t argc, char *argv[])
{
    fd_t fd = KFD_ERROR;

    if (argc < 2) {
        KPRINTF_ERROR("%s", "usage: touch <path>");
        return OK_TRUE;
    }
    fd = kfd_open(argv[1], KVFS_O_CREAT, 0644);
    if (fd == KFD_ERROR) {
        KPRINTF_ERROR("touch: cannot create the file");
        return OK_TRUE;
    }
    kfd_close(fd);
    return KO_FALSE;
}
