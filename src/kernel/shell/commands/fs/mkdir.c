/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/fs/mkdir
** File description:
** mkdir command source file
*/

#include <kernel/shell/shell.h>
#include <kernel/fs/fs_utils.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>
#include <defines.h>

/**
 * @brief Command for mkdir.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_mkdir(uint32_t argc, char *argv[])
{
    if (argc < 2) {
        KPRINTF_ERROR("usage: mkdir <path>");
        return OK_TRUE;
    }
    if (kmkdir(argv[1]) == -1) {
        KPRINTF_ERROR("mkdir: error while creating the directory");
        return OK_TRUE;
    }
    return KO_FALSE;
}
