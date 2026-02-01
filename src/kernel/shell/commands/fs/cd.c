/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/cd
** File description:
** cd command source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/sys/syscall.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>
#include <defines.h>

/**
 * @brief Command for cd.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_cd(uint32_t argc, char *argv[])
{
    int32_t r = -1;

    if (argc < 2) {
        r = kvfs_chdir("/");
    } else {
        r = kvfs_chdir(argv[1]);
    }
    if (r == -1) {
        KPRINTF_ERROR("cd: error while changing directory");
        return OK_TRUE;
    }
    return KO_FALSE;
}
