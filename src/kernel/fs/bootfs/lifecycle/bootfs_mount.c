/*
** DELOS PROJECT, 2025
** src/kernel/fs/bootfs/bootfs_mount
** File description:
** bootfs mount source file
*/

#include <kernel/fs/vfs/vfs_registry.h>
#include <kernel/memory/api/kmalloc.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/fs/bootfs/bootfs.h>
#include <kernel/fs/vfs/vfs.h>

/**
 * @brief Create the root entry for the bootfs.
 *
 * @param loc       The location of the root
 *
 * @return The entry pointer created, NULL otherwise.
 */
static bootfs_entry_t *
create_root_entry(const char *loc)
{
    bootfs_entry_t *root_entry = NULL;

    if (loc == NULL || kstrlen(loc) >= BOOTFS_NAME_MAX) {
        return NULL;
    }
    root_entry = kmalloc(sizeof(bootfs_entry_t));
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
 * @brief Create the vfs node and bootfs entry for the root directory.
 *
 * @param loc       The location of the mount
 * @param device    The device pointer on how to access data (unused on bootfs)
 *
 * @return The root directory of the bootfs file system ("/").
 */
vfs_node_t *
kbootfs_mount(const char *loc, void *device)
{
    bootfs_entry_t *root_entry = NULL;
    vfs_node_t *root_node = NULL;
    bool32_t from_device = KO_FALSE;

    if (device != NULL) {
        root_entry = (bootfs_entry_t *) device;
        from_device = OK_TRUE;
    } else {
        root_entry = create_root_entry(loc);
    }
    if (root_entry == NULL) {
        return NULL;
    }
    root_node = kbootfs_create_vfs_node(root_entry);
    if (root_node == NULL) {
        if (from_device == KO_FALSE) {
            kfree(root_entry);
        }
        return NULL;
    }
    return root_node;
}
