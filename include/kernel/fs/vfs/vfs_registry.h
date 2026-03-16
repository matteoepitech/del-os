/*
** DELOS PROJECT, 2025
** include/kernel/fs/vfs_registry
** File description:
** VFS registry header file
*/

#include <kernel/fs/vfs/vfs.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_FS_VFS_REGISTRY_H_
    #define KERNEL_FS_VFS_REGISTRY_H_
    
    #ifndef KVFS_REGISTRY_LEN
        #define KVFS_REGISTRY_LEN 1
    #endif /* ifndef KVFS_REGISTRY_LEN */

/* @brief Variable to get all available file system here */
extern vfs_fs_t kvfs_registry[KVFS_REGISTRY_LEN];

/**
 * @brief Get a file system driver structure by its name.
 *
 * @param name   The name of the file system to get ("ext2", "tar", "fat32", ...)
 *
 * @return The pointer to that file system driver structure.
 */
vfs_fs_t *
kvfs_get_fs(const char *name);

#endif /* ifndef KERNEL_FS_VFS_REGISTRY_H_ */
