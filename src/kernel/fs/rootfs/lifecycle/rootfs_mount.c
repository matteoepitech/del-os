/*
** DELOS PROJECT, 2025
** src/kernel/fs/rootfs/rootfs_mount
** File description:
** rootfs mount source file
*/

#include <kernel/fs/vfs/vfs_registry.h>
#include <kernel/memory/api/kmalloc.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/fs/rootfs/rootfs.h>
#include <kernel/fs/vfs/vfs.h>

/**
 * @brief Create the root entry for the rootfs.
 *
 * @param loc       The location of the root
 *
 * @return The entry pointer created, NULL otherwise.
 */
static rootfs_entry_t *
create_root_entry(const char *loc)
{
    rootfs_entry_t *root_entry = NULL;

    if (loc == NULL || kstrlen(loc) >= ROOTFS_NAME_MAX) {
        return NULL;
    }
    root_entry = kmalloc(sizeof(rootfs_entry_t));
    if (root_entry == NULL) {
        return NULL;
    }
    if (kstrcpy(root_entry->_name, loc) == NULL) {
        kfree(root_entry);
        return NULL;
    }
    root_entry->_size = 0;
    root_entry->_type = KVFS_STAT_IFDIR;
    return root_entry;
}

/**
 * @brief Create the vfs node and rootfs entry for the root directory.
 *
 * @param loc       The location of the mount
 * @param device    The device pointer on how to access data (unused on rootfs)
 *
 * @return The root directory of the rootfs file system ("/").
 */
vfs_node_t *
krootfs_mount(const char *loc, void *device)
{
    rootfs_entry_t *root_entry = NULL;
    vfs_node_t *root_node = NULL;
    bool32_t from_device = KO_FALSE;

    if (device != NULL) {
        root_entry = (rootfs_entry_t *) device;
        from_device = OK_TRUE;
    } else {
        root_entry = create_root_entry(loc);
    }
    if (root_entry == NULL) {
        return NULL;
    }
    root_node = krootfs_create_vfs_node(root_entry);
    if (root_node == NULL) {
        if (from_device == KO_FALSE) {
            kfree(root_entry);
        }
        return NULL;
    }
    return root_node;
}
