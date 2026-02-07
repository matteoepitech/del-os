/*
** DELOS PROJECT, 2025
** src/kernel/fs/tmpfs/tmpfs_read
** File description:
** TMPFS read source file
*/

#include <utils/kstdlib/kmemory.h>
#include <kernel/fs/tmpfs/tmpfs.h>
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
ktmpfs_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len)
{
    tmpfs_entry_t *entry = NULL;
    size_t read_content = 0;

    if (node == NULL || buffer == NULL || len == 0) {
        return 0;
    }
    entry = node->_private;
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_ISCHR(entry->_stat._mode) != KO_FALSE) {
        if (entry->_chr._read == NULL) {
            return 0;
        }
        return entry->_chr._read(node, offset, buffer, len);
    }
    if (KVFS_STAT_ISREG(entry->_stat._mode) == KO_FALSE) {
        KPRINTF_ERROR("tmpfs: cannot read on an entry which is not a file");
        return 0;
    }
    if (*offset > entry->_file._size) {
        return 0;
    }
    read_content = (entry->_file._size - *offset) > len ? len : (entry->_file._size - *offset);
    kmemcpy(buffer, &entry->_file._data_ptr[*offset], read_content);
    kvfs_stat_update_atime(&entry->_stat);
    *offset += read_content;
    return read_content;
}
