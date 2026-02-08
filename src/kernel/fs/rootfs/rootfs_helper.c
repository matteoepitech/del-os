/*
** DELOS PROJECT, 2025
** src/kernel/fs/rootfs/rootfs_helper
** File description:
** rootfs helper source file
*/

#include <kernel/fs/vfs/vfs_registry.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/fs/rootfs/rootfs.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/misc/panic.h>
#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>

/* @brief Variable containing a static version of all operations of rootfs fs */
vfs_ops_t krootfs_operations = {
    krootfs_read,
    NULL,
    krootfs_lookup,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    krootfs_stat
};

/**
 * @brief Get the structure containing every callback for that filesystem.
 *
 * @return Pointer to the vfs_ops_t structure for the rootfs fs.
 */
vfs_ops_t *
krootfs_get_operations(void)
{
    return &krootfs_operations;
}

/**
 * @brief Create a VFS node using an entry on that rootfs.
 *
 * @param entry     The entry to get data from
 *
 * @return The VFS node pointer created or NULL otherwise.
 */
vfs_node_t *
krootfs_create_vfs_node(rootfs_entry_t *entry)
{
    vfs_node_t *node = NULL;
    vfs_fs_t *fs = NULL;

    if (entry == NULL) {
        return NULL;
    }
    node = kmalloc(sizeof(vfs_node_t));
    if (node == NULL) {
        return NULL;
    }
    fs = kvfs_get_fs("rootfs");
    if (fs == NULL) {
        kfree(node);
        return NULL;
    }
    node->_fs = fs;
    node->_ops = krootfs_get_operations();
    node->_private = entry;
    node->_refcount = 1;
    return node;
}

/**
 * @brief Get the entry mode of an rootfs entry.
 *
 * @param entry  The entry of the file system
 *
 * @return The entry value type mode_t.
 */
mode_t
krootfs_get_entry_mode(const rootfs_entry_t *entry)
{
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_TYPE(entry->_type) != 0) {
        return entry->_type;
    }
    return entry->_type == 1 ? KVFS_STAT_IFDIR : KVFS_STAT_IFREG;
}
