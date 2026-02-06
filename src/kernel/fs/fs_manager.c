/*
** DELOS PROJECT, 2025
** src/kernel/fs/fs_manager
** File description:
** FS manager source file
*/

#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>
#include <kernel/fs/fs.h>
#include <defines.h>
#include <types.h>

/**
 * @brief Create the dev folder in the root folder and make some devices files.
 *
 * @param root   The root directory to make the dev folder
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_create_devices(vfs_node_t *root)
{
    vfs_node_t *dev_node = NULL;

    if (root == NULL) {
        return KO_FALSE;
    }
    root->_ops->_mkdir(root, "dev", 0755);
    dev_node = root->_ops->_lookup(root, "dev");
    if (dev_node == NULL) {
        return KO_FALSE;
    }
    if (dev_node->_ops->_create(dev_node, "stdout", KVFS_STAT_IFCHR | 0666) == KO_FALSE) {
        return KO_FALSE;
    }
    if (dev_node->_ops->_create(dev_node, "stderr", KVFS_STAT_IFCHR | 0666) == KO_FALSE) {
        return KO_FALSE;
    }
    if (dev_node->_ops->_create(dev_node, "stdin", KVFS_STAT_IFCHR | 0666) == KO_FALSE) {
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Init the file system stuff of the kernel.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_init(void)
{
    vfs_node_t *root = kvfs_mount("tmpfs", "/", NULL);

    if (root == NULL) {
        KPRINTF_ERROR("file_system: failed to mount the tmpfs");
        return KO_FALSE;
    }
    kvfs_cwd = root;
    if (kfs_create_devices(root) == KO_FALSE) {
        KPRINTF_ERROR("file_system: failed to create the dev folder entirely");
        return KO_FALSE;
    }
    KPRINTF_OK("file_system: dev folder created with success");
    KPRINTF_OK("file_system: successfully mounted the kernel's tmpfs");
    return OK_TRUE;
}
