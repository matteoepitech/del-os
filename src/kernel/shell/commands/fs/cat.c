/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/fs/cat
** File description:
** cat command source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Command for cat.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_cat(uint32_t argc, char *argv[])
{
    vfs_stat_t stat_buffer = {0};
    fd_t fd = KFD_ERROR;
    char *buffer = NULL;
    size_t len = 0;
    size_t read_len = 0;

    if (argc < 2) {
        KPRINTF_ERROR("usage: cat <path>");
        return OK_TRUE;
    }
    fd = kfd_open(argv[1], KVFS_O_RDONLY, 0);
    if (fd == KFD_ERROR) {
        KPRINTF_ERROR("cat: no such file or directory");
        return OK_TRUE;
    }
    if (kfd_stat(fd, &stat_buffer) == KO_FALSE) {
        kfd_close(fd);
        return OK_TRUE;
    }
    len = stat_buffer._size;
    if (len == 0) {
        kfd_close(fd);
        return KO_FALSE;
    }
    buffer = kmalloc(len + 1);
    if (buffer == NULL) {
        KPRINTF_ERROR("cat: failed to read, no more memory space available");
        kfd_close(fd);
        return OK_TRUE;
    }
    read_len = kfd_read(fd, buffer, len);
    buffer[read_len] = '\0';
    KPRINTF_OK("%s", argv[1]);
    ktty_puts(buffer, VGA_TEXT_DEFAULT_COLOR);
    ktty_putc('\n', VGA_TEXT_DEFAULT_COLOR);
    kfree(buffer);
    kfd_close(fd);
    return KO_FALSE;
}
