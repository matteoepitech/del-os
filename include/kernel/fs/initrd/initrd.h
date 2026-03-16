/*
** DELOS PROJECT, 2026
** include/kernel/fs/initrd/initrd
** File description:
** initrd header file
*/

#include <kernel/fs/vfs/vfs.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_FS_INITRD_H_
    #define KERNEL_FS_INITRD_H_

    #define KINITRD_NAME_MAX 64

/*
 * @brief Boot archive entry structure.
 *        - name = The absolute path of the entry
 *        - size = The size of the entry data for regular files
 *        - type = The type of the entry (legacy 0/1 values or VFS mode type bits)
 */
typedef struct initrd_entry {
    char _name[KINITRD_NAME_MAX];
    uint32_t _size;
    uint32_t _type;
} initrd_entry_t;

/**
 * @brief Check if an initrd image is at least large enough to contain one entry.
 *
 * @param image_size   The size of the embedded image
 *
 * @return OK_TRUE if the image can contain entries, KO_FALSE otherwise.
 */
bool32_t
kinitrd_is_size_valid(size_t image_size);

/**
 * @brief Unpack an initrd image into an already mounted writable VFS tree.
 *
 * @param root         The writable root node to populate
 * @param image        The pointer to the initrd image in memory
 * @param image_size   The size of the initrd image in bytes
 *
 * @return OK_TRUE if the whole image was unpacked, KO_FALSE otherwise.
 */
bool32_t
kinitrd_unpack(vfs_node_t *root, const void *image, size_t image_size);

#endif /* ifndef KERNEL_FS_INITRD_H_ */
