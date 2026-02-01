/*
** DELOS PROJECT, 2026
** src/kernel/fs/vfs/lifecycle/vfs_chdir
** File description:
** VFS chdir source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Change the directory of the kvfs_cwd.
 *
 * @param path   The path to change the directory
 *
 * @return 0 if worked, -1 if any error.
 */
int32_t
kvfs_chdir(const char *path)
{
    vfs_stat_t stat_buffer = {0};
    vfs_node_t *dir_to_cd = NULL;

    if (path == NULL) {
        return -1;
    }
    dir_to_cd = kvfs_lookup_open(path);
    if (dir_to_cd == NULL) {
        return -1;
    }
    if (kvfs_stat_from_node(dir_to_cd, &stat_buffer) == KO_FALSE) {
        kvfs_close(dir_to_cd);
        return -1;
    }
    if (KVFS_STAT_ISDIR(stat_buffer._mode) == KO_FALSE) {
        kvfs_close(dir_to_cd);
        return -1;
    }
    kvfs_close(kvfs_cwd);
    kvfs_cwd = dir_to_cd;
    return 0;
}
