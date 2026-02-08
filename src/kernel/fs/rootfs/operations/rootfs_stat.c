/*
** DELOS PROJECT, 2026
** src/kernel/fs/rootfs/rootfs_stat
** File description:
** rootfs stat source file
*/

#include <kernel/fs/vfs/vfs_stat.h>
#include <kernel/fs/rootfs/rootfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <defines.h>
#include <types.h>

/**
 * @brief Get metadata using the stat structure of a file or directory or such.
 *
 * @param node       The node of the file to get stat from
 * @param stat_ptr   The pointer to the stat buffer
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
krootfs_stat(vfs_node_t *node, vfs_stat_t *stat_ptr)
{
    rootfs_entry_t *entry = NULL;
    mode_t mode = 0;

    if (node == NULL || stat_ptr == NULL) {
        return KO_FALSE;
    }
    entry = node->_private;
    if (entry == NULL) {
        return KO_FALSE;
    }
    mode = krootfs_get_entry_mode(entry);
    if (KVFS_STAT_MODE_MASK_IT(mode) == 0) {
        mode |= KVFS_STAT_ISDIR(mode) ? 0555 : 0444;
    }
    *stat_ptr = kvfs_stat_create(mode);
    stat_ptr->_size = entry->_size;
    return OK_TRUE;
}
