/*
** DELOS PROJECT, 2025
** src/kernel/fs/bootfs/bootfs_read
** File description:
** bootfs read source file
*/

#include <utils/kstdlib/kmemory.h>
#include <kernel/fs/bootfs/bootfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>

/**
 * @brief Read data in the temporary virtual file system.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where to store the data read in (should be allocated to len size)
 * @param len        The number of byte to read
 *
 * @return The number of bytes read.
 */
size_t
kbootfs_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len)
{
    bootfs_entry_t *entry = NULL;
    uint8_t *data_ptr = NULL;
    size_t read_content = 0;

    if (node == NULL || offset == NULL || buffer == NULL || len == 0) {
        return 0;
    }
    entry = node->_private;
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_ISREG(kbootfs_get_entry_mode(entry)) == KO_FALSE) {
        KPRINTF_ERROR("bootfs: cannot read on an entry which is not a file");
        return 0;
    }
    if (*offset >= entry->_size) {
        return 0;
    }
    read_content = (entry->_size - *offset) > len ? len : (entry->_size - *offset);
    data_ptr = (uint8_t *) (entry + 1);
    kmemcpy(buffer, &data_ptr[*offset], read_content);
    *offset += read_content;
    return read_content;
}
