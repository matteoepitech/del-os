/*
** DELOS PROJECT, 2025
** include/kernel/fs/bootfs/bootfs
** File description:
** bootfs header file
*/

#include <kernel/fs/vfs/vfs_stat.h>
#include <kernel/fs/vfs/vfs.h>
#include <kernel/fs/dirent.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_FS_BOOTFS_H_
    #define KERNEL_FS_BOOTFS_H_

    #define BOOTFS_NAME_MAX 64

/*
 * @brief Boot file system entry structure.
 *        - name = The name of the entry
 *        - size = The size of the entry
 *        - type = The type of the entry (VFS mode type bits)
 */
typedef struct bootfs_entry {
    char _name[BOOTFS_NAME_MAX];
    uint32_t _size;
    uint32_t _type;
} bootfs_entry_t;

/* @brief Variable containing a static version of all operations of bootfs fs */
extern struct vfs_ops_s kbootfs_operations;

/**
 * @brief Create the vfs node and bootfs entry for the root directory.
 *
 * @param loc       The location of the mount
 * @param device    The device pointer on how to access data (unused on bootfs)
 *
 * @return The root directory of the bootfs file system ("/").
 */
vfs_node_t *
kbootfs_mount(const char *loc, void *device);

/**
 * @brief Read data in the temporary virtual file system.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where to store the data read in (should be allocated to len size)
 * @param len        The number of byte to read
 *
 * @return The number of bytes read.
 */
size_t
kbootfs_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len);

/**
 * @brief Get another node from the VFS with a node and next level path.
 *
 * @param node           The node of the file in the VFS
 * @param next_level     The next level of the node (e.g. we are at "/" we want the file "abc", next_level = "abc")
 *
 * @return The new VFS node we get in the next level.
 */
vfs_node_t *
kbootfs_lookup(vfs_node_t *node, const char *next_level);

/**
 * @brief Get metadata using the stat structure of a file or directory or such.
 *
 * @param node       The node of the file to get stat from
 * @param stat_ptr   The pointer to the stat buffer
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kbootfs_stat(vfs_node_t *node, vfs_stat_t *stat_ptr);

/**
 * @brief Get the structure containing every callback for that filesystem.
 *
 * @return Pointer to the vfs_ops_t structure for the bootfs fs.
 */
vfs_ops_t *
kbootfs_get_operations(void);

/**
 * @brief Create a VFS node using an entry on that bootfs.
 *
 * @param entry     The entry to get data from
 *
 * @return The VFS node pointer created or NULL otherwise.
 */
vfs_node_t *
kbootfs_create_vfs_node(bootfs_entry_t *entry);

/**
 * @brief Get the entry mode of an bootfs entry.
 *
 * @param entry  The entry of the file system
 *
 * @return The entry value type mode_t.
 */
mode_t
kbootfs_get_entry_mode(const bootfs_entry_t *entry);

#endif /* ifndef KERNEL_FS_BOOTFS_H_ */
