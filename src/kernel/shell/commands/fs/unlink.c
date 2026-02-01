/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/fs/unlink
** File description:
** unlink command source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/shell/shell.h>
#include <kernel/sys/syscall.h>
#include <utils/misc/print.h>
#include <defines.h>

/**
 * @brief Command for unlink.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_unlink(uint32_t argc, char *argv[])
{
    if (argc < 2) {
        KPRINTF_ERROR("%s", "usage: unlink <path>");
        return OK_TRUE;
    }
    if (kvfs_unlink(argv[1]) == KO_FALSE) {
        KPRINTF_ERROR("unlink: error while unlinking");
        return OK_TRUE;
    }
    return KO_FALSE;
}
