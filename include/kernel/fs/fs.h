/*
** DELOS PROJECT, 2025
** include/kernel/fs/fs
** File description:
** FS header file
*/

#include <kernel/fs/vfs/vfs.h>
#include <types.h>

#ifndef KERNEL_FS_H_
    #define KERNEL_FS_H_

/**
 * @brief Init the file system stuff of the kernel.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_init(void);

/**
 * @brief Create the dev folder in the root folder and make some devices files.
 *
 * @param root   The root directory to make the dev folder
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_create_devices(vfs_node_t *root);

#endif /* ifndef KERNEL_FS_H_ */
