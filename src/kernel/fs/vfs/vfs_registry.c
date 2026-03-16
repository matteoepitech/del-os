/*
** DELOS PROJECT, 2025
** src/kernel/fs/vfs/vfs_registry
** File description:
** VFS registry source file
*/

#include <kernel/fs/vfs/vfs_registry.h>
#include <kernel/fs/tmpfs/tmpfs.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/fs/vfs/vfs.h>

/* @brief Variable to get all available file system here */
vfs_fs_t kvfs_registry[KVFS_REGISTRY_LEN] = {
    {"tmpfs", ktmpfs_mount}
};

/**
 * @brief Get a file system driver structure by its name.
 *
 * @param name   The name of the file system to get ("ext2", "tar", "fat32", ...)
 *
 * @return The pointer to that file system driver structure.
 */
vfs_fs_t *
kvfs_get_fs(const char *name)
{
    if (name == NULL) {
        return NULL;
    }
    for (uint32_t i = 0; i < KVFS_REGISTRY_LEN; i++) {
        if (kstrcmp(kvfs_registry[i]._name, name) == 0) {
            return &kvfs_registry[i];
        }
    }
    return NULL;
}
