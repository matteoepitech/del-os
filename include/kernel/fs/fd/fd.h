/*
** DELOS PROJECT, 2026
** include/kernel/fs/fd/fd
** File description:
** FD header file
*/

#include <kernel/fs/vfs/vfs.h>
#include <types.h>

#ifndef KERNEL_FS_FD_H_
    #define KERNEL_FS_FD_H_

    #ifndef KFD_MAX_COUNT
        #define KFD_MAX_COUNT 32
    #endif /* ifndef KFD_MAX_COUNT */

    #ifndef KFD_ERROR
        #define KFD_ERROR -1 
    #endif /* ifndef KFD_ERROR */

/* Forward declaration for process_s */
struct process_s;

/*
 * @brief Structure for a file descriptor.
 *        - node     = the VFS node
 *        - offset   = the current offset to the file
 *        - flags    = the flags
 *        - refcount = the reference count
 */
typedef struct file_desc_s {
    vfs_node_t *_node;
    off_t _offset;
    uint32_t _flags;
    uint32_t _refcount;
} file_desc_t;

/*
 * @brief Enumerations for accessing FD.
 *        - KFD_ACCESS_READ  = need to read
 *        - KFD_ACCESS_WRITE = need to write
 */
typedef enum {
    KFD_ACCESS_READ  = 1 << 0,
    KFD_ACCESS_WRITE = 1 << 1,
} fd_access_t;

/**
 * @brief Create a file descriptor from a VFS node.
 *
 * @param node   The VFS node
 * @param flags  The flags used to open the descriptor
 *
 * @return The file descriptor created from the node.
 */
fd_t
kfd_create(vfs_node_t *node, int32_t flags);

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
kfd_create_for_process(struct process_s *process, vfs_node_t *node, int32_t flags);

/**
 * @brief Get the information about a FD.
 *
 * @param fd     The FD to get
 *
 * @return The structure file descriptor pointer.
 */
file_desc_t *
kfd_get(fd_t fd);

/**
 * @brief Get the information about a file descriptor for a process.
 *
 * @param process    The process
 * @param fd         The file descriptor
 *
 * @return The file descriptor information.
 */
file_desc_t *
kfd_get_from_process(struct process_s *process, fd_t fd);

/**
 * @brief Check if a file descriptor has the required access permissions.
 *
 * @param fd              The file descriptor index.
 * @param required_access The minimum access required (KFD_ACCESS_*).
 *
 * @return OK_TRUE if access is allowed, KO_FALSE otherwise.
 */
bool32_t
kfd_check_access(fd_t fd, fd_access_t required_access);

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
kfd_check_access_from_process(struct process_s *process, fd_t fd, fd_access_t required_access);

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
kfd_open(const char *path, int32_t flags, mode_t mode);

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
kfd_open_for_process(struct process_s *process, const char *path, int32_t flags, mode_t mode);

/**
 * @brief Close a file descriptor and drop its reference to the VFS node.
 *
 * @param fd   The file descriptor to close
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfd_close(fd_t fd);

/**
 * @brief Close a FD of a process.
 *
 * @param process    The process to close the FD from
 * @param fd         The FD to close
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kfd_close_for_process(struct process_s *process, fd_t fd);

#endif /* ifndef KERNEL_FS_FD_H_ */
