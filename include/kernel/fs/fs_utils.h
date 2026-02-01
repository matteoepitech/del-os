/*
** DELOS PROJECT, 2025
** include/kernel/fs/fs_utils
** File description:
** FS utils header file
*/

#include <kernel/fs/fd/fd_operations.h>
#include <types.h>

#ifndef KERNEL_FS_UTILS_H_
    #define KERNEL_FS_UTILS_H_

/**
 * @brief Create a directory.
 *
 * @param path   The path of the directory to create
 *
 * @return 0 if worked, -1 on error.
 */
int32_t
kmkdir(const char *path);

/**
 * @brief Remove a directory.
 *
 * @param path   The path to the directory to remove
 *
 * @return 0 if worked, -1 on error.
 */
int32_t
krmdir(const char *path);

/**
 * @brief Get informations about a file and save it to a structure.
 *
 * @param path       The path of the file to get informations
 * @param stat_ptr   The stat structure pointer to get informations
 *
 * @return 0 if worked, -1 on errror.
 */
int32_t
kstat(const char *path, vfs_stat_t *stat_ptr);

#endif /* ifndef KERNEL_FS_UTILS_H_ */
