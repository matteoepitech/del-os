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
 * @brief Create the device files in the /dev tmpfs mount.
 *
 * @param dev_node   The /dev mounted directory node
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_create_devices(vfs_node_t *dev_node);

#endif /* ifndef KERNEL_FS_H_ */
