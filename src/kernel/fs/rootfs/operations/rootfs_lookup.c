/*
** DELOS PROJECT, 2025
** src/kernel/fs/rootfs/rootfs_lookup
** File description:
** rootfs lookup source file
*/

#include <utils/kstdlib/kstring.h>
#include <kernel/fs/rootfs/rootfs.h>
#include <kernel/fs/vfs/vfs.h>
#include <utils/misc/print.h>

/**
 * @brief Get another node from the VFS with a node and next level path.
 *
 * @param node           The node of the file in the VFS
 * @param next_level     The next level of the node (e.g. we are at "/" we want the file "abc", next_level = "abc")
 *
 * @return The new VFS node we get in the next level.
 */
vfs_node_t *
krootfs_lookup(vfs_node_t *node, const char *next_level)
{
    rootfs_entry_t *entry = NULL;

    if (node == NULL || next_level == NULL || node->_private == NULL) {
        return NULL;
    }
    entry = (rootfs_entry_t *) node->_private;
    if (KVFS_STAT_ISDIR(krootfs_get_entry_mode(entry)) == KO_FALSE) {
        return NULL;
    }
    if (kstrcmp(next_level, ".") == 0) {
        return krootfs_create_vfs_node(entry);
    } else if (kstrcmp(next_level, "..") == 0) {
        return krootfs_create_vfs_node(entry);
    }
    return NULL;
}
