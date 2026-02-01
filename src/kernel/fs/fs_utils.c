/*
** DELOS PROJECT, 2026
** src/kernel/fs/fs_utils
** File description:
** FS utils source file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <kernel/fs/vfs/vfs_open.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/sys/syscall.h>
#include <kernel/fs/fs_utils.h>
#include <kernel/fs/fd/fd.h>
#include <defines.h>

/**
 * @brief Create a directory.
 *
 * @param path   The path of the directory to create
 *
 * @return 0 if worked, -1 on error.
 */
int32_t
kmkdir(const char *path)
{
    fd_t dir = KFD_ERROR;

    if (kstrcmp(path, "..") == 0 || kstrcmp(path, ".") == 0 || kstrcmp(path, "/") == 0) {
        return -1;
    }
    dir = kfd_open(path, KVFS_O_CREAT | KVFS_O_DIRECTORY | KVFS_O_EXCL, 0666);
    if (dir != KFD_ERROR) {
        kfd_close(dir);
        return 0;
    }
    return -1;
}

/**
 * @brief Remove a directory.
 *
 * @param path   The path to the directory to remove
 *
 * @return 0 if worked, -1 on error.
 */
int32_t
krmdir(const char *path)
{
    bool32_t rmdir_r = KO_FALSE;

    if (kstrcmp(path, "..") == 0 || kstrcmp(path, ".") == 0 || kstrcmp(path, "/") == 0) {
        return -1;
    }
    rmdir_r = kvfs_rmdir(path);
    if (rmdir_r == KO_FALSE) {
        return -1;
    }
    return 0;
}

/**
 * @brief Get informations about a file and save it to a structure.
 *
 * @param path       The path of the file to get informations
 * @param stat_ptr   The stat structure pointer to get informations
 *
 * @return 0 if worked, -1 on errror.
 */
int32_t
kstat(const char *path, vfs_stat_t *stat_ptr)
{
    fd_t fd = KFD_ERROR;

    if (path == NULL || stat_ptr == NULL) {
        return -1;
    }
    fd = kfd_open(path, KVFS_O_RDONLY, 0);
    if (fd == -1) {
        return -1;
    }
    if (kfd_stat(fd, stat_ptr) == KO_FALSE) {
        return -1;
    }
    return 0;
}
