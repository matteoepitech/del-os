/*
** DELOS PROJECT, 2025
** src/kernel/fs/fs_manager
** File description:
** FS manager source file
*/

#include <drivers/input/keyboard_char_device.h>
#include <kernel/fs/rootfs/rootfs.h>
#include <kernel/fs/tmpfs/tmpfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <drivers/video/vga.h>
#include <utils/misc/print.h>
#include <kernel/fs/fs.h>
#include <defines.h>
#include <types.h>

/* @brief Rootfs symbols made using the i386-elf-objcopy tool */
extern char _binary_rootfs_img_start[];
extern char _binary_rootfs_img_end[];

/**
 * @brief Setup the files using the VGA driver first.
 *
 * @param dev_node   The device node ("/dev/")
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
kfs_setup_devices(vfs_node_t *dev_node)
{
    tmpfs_entry_t *entry = NULL;
    vfs_node_t *tmp = NULL;

    if (dev_node == NULL) {
        return KO_FALSE;
    }
    tmp = dev_node->_ops->_lookup(dev_node, "stdout");
    if (tmp == NULL || tmp->_private == NULL) {
        kvfs_close(tmp);
        return KO_FALSE;
    }
    entry = (tmpfs_entry_t *) tmp->_private;
    entry->_chr._write = kfs_vga_write;
    kvfs_close(tmp);
    tmp = dev_node->_ops->_lookup(dev_node, "stderr");
    if (tmp == NULL || tmp->_private == NULL) {
        kvfs_close(tmp);
        return KO_FALSE;
    }
    entry = (tmpfs_entry_t *) tmp->_private;
    entry->_chr._write = kfs_vga_write;
    kvfs_close(tmp);
    tmp = dev_node->_ops->_lookup(dev_node, "stdin");
    if (tmp == NULL || tmp->_private == NULL) {
        kvfs_close(tmp);
        return KO_FALSE;
    }
    entry = (tmpfs_entry_t *) tmp->_private;
    entry->_chr._read = kfs_keyboard_read;
    kvfs_close(tmp);
    return OK_TRUE;
}

/**
 * @brief Create the device files in the mounted /dev tmpfs.
 *
 * @param dev_node   The /dev node
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_create_devices(vfs_node_t *dev_node)
{
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
    return kfs_setup_devices(dev_node);
}

/**
 * @brief Init the file system stuff of the kernel.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfs_init(void)
{
    vfs_node_t *dev_node = NULL;
    vfs_node_t *root = NULL;

    if ((size_t) (_binary_rootfs_img_end - _binary_rootfs_img_start) < sizeof(rootfs_entry_t)) {
        KPRINTF_ERROR("file_system: invalid embedded rootfs image");
        return KO_FALSE;
    }
    root = kvfs_mount("rootfs", "/", _binary_rootfs_img_start);
    if (root == NULL) {
        KPRINTF_ERROR("file_system: failed to mount the rootfs");
        return KO_FALSE;
    }
    kvfs_cwd = root;
    dev_node = ktmpfs_mount("/dev", NULL);
    if (dev_node == NULL) {
        KPRINTF_ERROR("file_system: failed to mount tmpfs on /dev");
        return KO_FALSE;
    }
    if (kfs_create_devices(dev_node) == KO_FALSE) {
        KPRINTF_ERROR("file_system: failed to create the dev files");
        return KO_FALSE;
    }
    KPRINTF_OK("file_system: tmpfs mounted on /dev");
    KPRINTF_OK("file_system: device files created with success");
    KPRINTF_OK("file_system: successfully mounted the kernel's rootfs");
    return OK_TRUE;
}
