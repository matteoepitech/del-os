/*
** DELOS PROJECT, 2026
** include/drivers/input/keyboard_char_device
** File description:
** Keyboard char device header file
*/

#include <kernel/fs/vfs/vfs.h>
#include <defines.h>
#include <types.h>

#ifndef DRIVER_INPUT_KEYBOARD_CHAR_DEVICE_H_
    #define DRIVER_INPUT_KEYBOARD_CHAR_DEVICE_H_

/**
 * @brief Get the data from the keyboard. Used in the stdin file.
 *
 * @param node       The node to read the data from
 * @param offset     The current offset in the fd
 * @param buffer     The buffer to put the data to
 * @param len        The len of the buffer
 *
 * @return Number of bytes read.
 */
size_t
kfs_keyboard_read(vfs_node_t *node, off_t *offset, void *buffer, size_t len);

#endif /* ifndef DRIVER_INPUT_KEYBOARD_CHAR_DEVICE_H_ */
