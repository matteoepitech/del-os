/*
** DELOS PROJECT, 2025
** include/kernel/fs/tmpfs/tmpfs
** File description:
** TMPFS header file
*/

#include <kernel/fs/vfs/vfs_stat.h>
#include <kernel/fs/vfs/vfs.h>
#include <kernel/fs/dirent.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_FS_TMPFS_H_
    #define KERNEL_FS_TMPFS_H_

/*
 * @brief Structure for a file entry in the tmpfs file system.
 *        - name     = the name of the entry (max len KTMPFS_NAME_MAX_LEN)
 *        - stat     = the stat metadata of the entry
 *        - parent   = the parent of the entry (likely a dir)
 *        - data_ptr = the pointer to the raw data in memory (file type)
 *        - size     = the size of the raw data in memory (file type)
 *        - child    = the first child entry of a dir (dir type) then use the next to go through entire dir
 *        - next     = the next occurence in the entry (on a dir for example)
 */
typedef struct tmpfs_entry_s {
    char _name[KVFS_MAX_NAME_LEN];
    vfs_stat_t _stat;
    struct tmpfs_entry_s *_parent;
    union {
        struct {
            uint8_t *_data_ptr;
            size_t _size;
            size_t _capacity;
        } _file;
        struct {
            size_t (*_read)(vfs_node_t *node, off_t *offset, void *buffer, size_t len);
            size_t (*_write)(vfs_node_t *node, off_t *offset, const void *buffer, size_t len);
            void *_ctx;
        } _chr;
        struct {
            struct tmpfs_entry_s *_child;
        } _dir;
    };
    struct tmpfs_entry_s *_next;
} tmpfs_entry_t;

/* @brief Variable containing a static version of all operations of TMPFS fs */
extern struct vfs_ops_s ktmpfs_operations;

/**
 * @brief Create the vfs node and tmpfs entry for the root directory.
 *
 * @param loc       The location of the mount
 * @param device    The device pointer on how to access data (unused on tmpfs)
 *
 * @return The root directory of the tmpfs file system ("/").
 */
vfs_node_t *
ktmpfs_mount(const char *loc, UNUSED void *device);

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
ktmpfs_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len);

/**
 * @brief Write data in the temporary virtual file system.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where the data to write is in
 * @param len        The number of bytes to write
 *
 * @return The size of the number of bytes write in the file.
 */
size_t
ktmpfs_write(vfs_node_t *node, off_t *offset, const void *buffer, size_t len);

/**
 * @brief Get another node from the VFS with a node and next level path.
 *
 * @param node           The node of the file in the VFS
 * @param next_level     The next level of the node (e.g. we are at "/" we want the file "abc", next_level = "abc")
 *
 * @return The new VFS node we get in the next level.
 */
vfs_node_t *
ktmpfs_lookup(vfs_node_t *node, const char *next_level);

/**
 * @brief Create a file in a parent entry on the TMPFS.
 *
 * @param parent     The parent of the file (likely a dir)
 * @param name       The name of the file we want to create
 * @param mode       The file mode
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_create(vfs_node_t *parent, const char *name, mode_t mode);

/**
 * @brief Create a directory in a parent entry on the TMPFS.
 *
 * @param parent     The parent of the dir (likely a dir)
 * @param name       The name of the dir we want to mkdir
 * @param mode       The mode for the directory
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_mkdir(vfs_node_t *parent, const char *name, mode_t mode);

/**
 * @brief Remove a directory only and only when its content is empty.
 *
 * @param dir    The directory VFS node to delete
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_rmdir(vfs_node_t *dir);

/**
 * @brief Get metadata using the stat structure of a file or directory or such.
 *
 * @param node       The node of the file to get stat from
 * @param stat_ptr   The pointer to the stat buffer
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_stat(vfs_node_t *node, vfs_stat_t *stat_ptr);

/**
 * @brief Iterate through the VFS node directory.
 *
 * @param dir        The directory strcuture (VFS node)
 * @param offset     The current index in the directory (iterator way)
 * @param dirent     A pointer to the structure to fill out of informations
 *
 * @return OK_TRUE if worked, KO_FALSE othewise.
 */
bool32_t
ktmpfs_readdir(vfs_node_t *dir, off_t *offset, dirent_t *dirent);

/**
 * @brief Unlink a file (remove it's entry but not his data of course)
 *
 * @param node   The node to remove its entry
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_unlink(vfs_node_t *node);

/**
 * @brief Get the structure containing every callback for that filesystem.
 *
 * @return Pointer to the vfs_ops_t structure for the TMPFS fs.
 */
vfs_ops_t *
ktmpfs_get_operations(void);

/**
 * @brief Create a VFS node using an entry on that tmpfs.
 *
 * @param entry     The entry to get data from
 *
 * @return The VFS node pointer created or NULL otherwise.
 */
vfs_node_t *
ktmpfs_create_vfs_node(tmpfs_entry_t *entry);

/**
 * @brief Create an entry on the TMPFS fs using parent, name, type and return it.
 *
 * @param parent     The parent of the entry
 * @param name       The name of the entry
 * @param mode       The mode of the entry
 *
 * @return The entry created.
 */
tmpfs_entry_t *
ktmpfs_create_entry(tmpfs_entry_t *parent, const char *name, mode_t mode);

/**
 * @brief Remove the entry from the parent linked list childs.
 *
 * @param parent     The pointer to the parent of that entry
 * @param entry      The pointer to the entry to remove
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
ktmpfs_remove_from_parent_ll(tmpfs_entry_t *parent, tmpfs_entry_t *entry);

#endif /* ifndef KERNEL_FS_TMPFS_H_ */
