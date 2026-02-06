/*
** DELOS PROJECT, 2026
** src/drivers/video/vga_char_device
** File description:
** VGA Char device source file
*/

#include <kernel/tty/tty.h>
#include <kernel/fs/tmpfs/tmpfs.h>

/**
 * @brief Write data in the VGA buffer using a char device.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where the data to write is in
 * @param len        The number of bytes to write
 *
 * @return The size of the number of bytes write in the file.
 */
size_t
kfs_vga_write(vfs_node_t *node, off_t *offset, const void *buffer, size_t len)
{
    tmpfs_entry_t *entry = NULL;
    const char *bytes = buffer;

    if (node == NULL || buffer == NULL || len == 0) {
        return 0;
    }
    entry = node->_private;
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_ISCHR(entry->_stat._mode) == KO_FALSE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        ktty_putc(bytes[i], VGA_TEXT_DEFAULT_COLOR);
    }
    if (offset != NULL) {
        *offset += len;
    }
    kvfs_stat_update_mtime(&entry->_stat);
    kvfs_stat_update_ctime(&entry->_stat);
    return len;
}
