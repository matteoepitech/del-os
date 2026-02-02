/*
** DELOS PROJECT, 2026
** src/kernel/fs/fd/operations/fd_close
** File description:
** FD close source file
*/

#include <kernel/memory/api/kmalloc.h>
#include <kernel/fs/fd/fd.h>
#include <kernel/scheduler/scheduler.h>
#include <kernel/scheduler/process.h>
#include <defines.h>

/**
 * @brief Close a file descriptor and drop its reference to the VFS node.
 *
 * @param fd   The file descriptor to close
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfd_close(fd_t fd)
{
    return kfd_close_for_process(ktask_current ? ktask_current->_process : NULL, fd);
}

/**
 * @brief Close a FD of a process.
 *
 * @param process    The process to close the FD from
 * @param fd         The FD to close
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfd_close_for_process(process_t *process, fd_t fd)
{
    file_desc_t *fd_struct = kfd_get_from_process(process, fd);
    file_desc_t **table = NULL;

    if (fd_struct == NULL || process == NULL) {
        return KO_FALSE;
    }
    table = process->_fds;
    if (fd_struct->_refcount > 1) {
        fd_struct->_refcount--;
        return OK_TRUE;
    }
    kvfs_close(fd_struct->_node);
    kfree(fd_struct);
    table[fd] = NULL;
    return OK_TRUE;
}
