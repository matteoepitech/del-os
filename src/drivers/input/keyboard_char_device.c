/*
** DELOS PROJECT, 2026
** src/drivers/input/keyboard_char_device
** File description:
** Keyboard char device driver source file
*/

#include <drivers/input/keyboard_char_device.h>
#include <kernel/misc/keyboard.h>
#include <kernel/fs/tmpfs/tmpfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <defines.h>
#include <types.h>

/**
 * @brief Get the data from the keyboard. Used in the stdin file.
 *
 * @param node       The node to read the data from
 * @param offset     The current offset in the fd
 * @param buffer     The buffer to put the data to
 * @param len        The len of the buffer
 *
 * @return Number of bytes read.
 */
size_t
kfs_keyboard_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len)
{
    tmpfs_entry_t *entry = NULL;
    char *bytes = buffer;
    size_t read_len = 0;

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
    while (read_len < len) {
        bytes[read_len++] = kkeyboard_getchar();
    }
    if (offset != NULL) {
        *offset += read_len;
    }
    kvfs_stat_update_atime(&entry->_stat);
    return read_len;
}
