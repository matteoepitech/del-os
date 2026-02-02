/*
** DELOS PROJECT, 2026
** src/kernel/fs/fd/operations/fd_open
** File description:
** FD open source file
*/

#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/fs/fd/fd.h>
#include <kernel/scheduler/scheduler.h>
#include <kernel/scheduler/process.h>

/**
 * @brief Open a file and go through its entire path to get the node associated to the end level.
 *
 * @param path   The complete path to a node (e.g. "/abc/dir/a.txt")
 * @param flags  The flags O_CREAT | O_RDONLY ...
 * @param mode   The mode (used when creating the file only)
 *
 * @return The file descriptor.
 */
fd_t
kfd_open(const char *path, int32_t flags, mode_t mode)
{
    return kfd_open_for_process(ktask_current ? ktask_current->_process : NULL, path, flags, mode);
}

/**
 * @brief Open a file and go through its entire path to get the node associated to the end level.
 *
 * @param process The process to open from
 * @param path    The complete path to a node (e.g. "/abc/dir/a.txt")
 * @param flags   The flags O_CREAT | O_RDONLY ...
 * @param mode    The mode (used when creating the file only)
 *
 * @return The file descriptor.
 */
fd_t
kfd_open_for_process(process_t *process, const char *path, int32_t flags, mode_t mode)
{
    vfs_node_t *node = NULL;

    if (path == NULL || process == NULL) {
        return KFD_ERROR;
    }
    node = kvfs_open(path, flags, mode);
    if (node == NULL) {
        return KFD_ERROR;
    }
    return kfd_create_for_process(process, node, flags);
}
