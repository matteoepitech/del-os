/*
** DELOS PROJECT, 2025
** src/kernel/fs/tmpfs/tmpfs_write
** File description:
** TMPFS write source file
*/

#include <kernel/memory/api/kmalloc.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/fs/tmpfs/tmpfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>

/**
 * @brief Write data in the temporary virtual file system.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where the data to write is in
 * @param len        The number of bytes to write
 *
 * @return The size of the number of bytes write in the file.
 */
size_t
ktmpfs_write(vfs_node_t *node, off_t *offset, const void *buffer, size_t len)
{
    tmpfs_entry_t *entry = NULL;
    void *tmp_ptr = NULL;
    size_t new_size = 0;

    if (node == NULL || buffer == NULL || len == 0) {
        return 0;
    }
    entry = node->_private;
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_ISCHR(entry->_stat._mode) != KO_FALSE) {
        if (entry->_chr._write == NULL) {
            return 0;
        }
        return entry->_chr._write(node, offset, buffer, len);
    }
    if (KVFS_STAT_ISREG(entry->_stat._mode) == KO_FALSE) {
        KPRINTF_ERROR("tmpfs: cannot write on an entry which is not a file");
        return 0;
    }
    new_size = *offset + len;
    if (new_size > entry->_file._capacity) {
        tmp_ptr = krealloc(entry->_file._data_ptr, new_size);
        if (tmp_ptr == NULL) {
            KPRINTF_ERROR("tmpfs: failed to write on an entry, due to a failed realloc");
            return 0;
        }
        entry->_file._data_ptr = tmp_ptr;
        entry->_file._capacity = new_size;
    }
    kmemcpy(&entry->_file._data_ptr[*offset], buffer, len);
    entry->_file._size = MAX(new_size, entry->_file._size);
    kvfs_stat_update_size(&entry->_stat, entry->_file._size);
    *offset += len;
    return len;
}
