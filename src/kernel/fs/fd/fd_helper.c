/*
** DELOS PROJECT, 2026
** src/kernel/fs/fd/fd_helper
** File description:
** FD helper source file
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/memory/api/kmalloc.h>
#include <kernel/scheduler/process.h>
#include <kernel/fs/vfs/vfs_open.h>
#include <kernel/fs/fd/fd.h>

/**
 * @brief Get the fd table of a process.
 *
 * @param process    The process to get the file table from
 *
 * @return The array of file descriptor.
 */
static file_desc_t **
kfd_get_table_process(process_t *process)
{
    if (process == NULL) {
        return NULL;
    }
    return process->_fds;
}

/**
 * @brief Create a file descriptor from a VFS node.
 *
 * @param node   The VFS node
 * @param flags  The flags used to open the descriptor
 *
 * @return The file descriptor created from the node.
 */
fd_t
kfd_create(vfs_node_t *node, int32_t flags)
{
    return kfd_create_for_process(ktask_current ? ktask_current->_process : NULL, node, flags);
}

/**
 * @brief Create a file descriptor for a process.
 *
 * @param process    The process to get the file descriptor
 * @param node       The node of the VFS
 * @param flags      The flag of the node to open
 *
 * @return The file descriptor created.
 */
fd_t
kfd_create_for_process(process_t *process, vfs_node_t *node, int32_t flags)
{
    file_desc_t *fd_struct = NULL;
    file_desc_t **table = kfd_get_table_process(process);
    fd_t fd = KFD_ERROR;

    if (node == NULL || table == NULL) {
        return KFD_ERROR;
    }
    for (fd = 0; fd < KFD_MAX_COUNT; fd++) {
        if (table[fd] == NULL) {
            break;
        }
    }
    if (fd >= KFD_MAX_COUNT) {
        return KFD_ERROR;
    }
    fd_struct = kmalloc(sizeof(file_desc_t));
    if (fd_struct == NULL) {
        return KFD_ERROR;
    }
    fd_struct->_node = node;
    fd_struct->_offset = 0;
    fd_struct->_flags = flags;
    fd_struct->_refcount = 1;
    table[fd] = fd_struct;
    return fd;
}

/**
 * @brief Get the information about a FD.
 *
 * @param fd     The FD to get
 *
 * @return The structure file descriptor pointer.
 */
file_desc_t *
kfd_get(fd_t fd)
{
    return kfd_get_from_process(ktask_current ? ktask_current->_process : NULL, fd);
}

/**
 * @brief Get the information about a file descriptor for a process.
 *
 * @param process    The process
 * @param fd         The file descriptor
 *
 * @return The file descriptor information.
 */
file_desc_t *
kfd_get_from_process(process_t *process, fd_t fd)
{
    file_desc_t **table = kfd_get_table_process(process);

    if (fd < 0 || fd >= KFD_MAX_COUNT || table == NULL) {
        return NULL;
    }
    return table[fd];
}

/**
 * @brief Check if a file descriptor has the required access permissions.
 *
 * @param fd              The file descriptor index.
 * @param required_access The minimum access required (KFD_ACCESS_*).
 *
 * @return OK_TRUE if access is allowed, KO_FALSE otherwise.
 */
bool32_t
kfd_check_access(fd_t fd, fd_access_t required_access)
{
    return kfd_check_access_from_process(ktask_current ? ktask_current->_process : NULL, fd, required_access);
}

/**
 * @brief Check if the access is granted for a process and a fd.
 *
 * @param process            The process
 * @param fd                 The file descriptor
 * @param required_access    The minimum access required
 *
 * @return OK_TRUE if access is allowed, KO_FALSE otherwise.
 */
bool32_t
kfd_check_access_from_process(process_t *process, fd_t fd, fd_access_t required_access)
{
    file_desc_t *desc = NULL;
    uint32_t allowed = 0;
    int32_t accmode = 0;
    file_desc_t **table = kfd_get_table_process(process);

    if (fd < 0 || fd >= KFD_MAX_COUNT || table == NULL) {
        return KO_FALSE;
    }
    desc = table[fd];
    if (desc == NULL) {
        return KO_FALSE;
    }
    accmode = desc->_flags & KVFS_O_ACCMODE;
    if (accmode == KVFS_O_WRONLY) {
        allowed |= KFD_ACCESS_WRITE;
    } else if (accmode == KVFS_O_RDWR) {
        allowed |= KFD_ACCESS_READ | KFD_ACCESS_WRITE;
    } else {
        allowed |= KFD_ACCESS_READ;
    }
    if ((allowed & required_access) != required_access) {
        return KO_FALSE;
    }
    return OK_TRUE;
}
