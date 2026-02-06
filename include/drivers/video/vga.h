/*
** DELOS PROJECT, 2025
** include/drivers/video/vga
** File description:
** VGA Header file
*/

#include <kernel/memory/vmm/vmm.h>
#include <kernel/fs/vfs/vfs.h>
#include <math/2d/point.h>

#ifndef DRIVER_VIDEO_VGA_H_
    #define DRIVER_VIDEO_VGA_H_

    #ifndef VGA_TEXT_MODE_START_MMIO_ADDR
        #define VGA_TEXT_MODE_START_MMIO_ADDR 0xb8000
        #define VGA_MMIO_ADDR ((volatile uint8_t *) PHYS_TO_VIRT(VGA_TEXT_MODE_START_MMIO_ADDR))
    #endif /* ifndef VGA_TEXT_MODE_START_MMIO_ADDR */

    #ifndef VGA_TEXT_MODE_CURSOR_REGISTER_ADDR
        #define VGA_TEXT_MODE_CURSOR_REGISTER_ADDR 0x3d4
    #endif /* ifndef VGA_TEXT_MODE_CURSOR_REGISTER_ADDR */

    #ifndef VGA_TEXT_MODE_CURSOR_DATA_ADDR
        #define VGA_TEXT_MODE_CURSOR_DATA_ADDR 0x3d5
    #endif /* ifndef VGA_TEXT_MODE_CURSOR_DATA_ADDR */

    #ifndef VGA_COLUMNS_MAX
        #define VGA_COLUMNS_MAX 80
    #endif /* ifndef VGA_COLUMNS_MAX */

    #ifndef VGA_LINES_MAX
        #define VGA_LINES_MAX 25
    #endif /* ifndef VGA_LINES_MAX */

    #ifndef VGA_COLORS_VALUES_H_
        #define VGA_COLORS_VALUES_H_

        #define VGA_BLACK_FOREGROUND         0x00
        #define VGA_BLUE_FOREGROUND          0x01
        #define VGA_GREEN_FOREGROUND         0x02
        #define VGA_CYAN_FOREGROUND          0x03
        #define VGA_RED_FOREGROUND           0x04
        #define VGA_MAGENTA_FOREGROUND       0x05
        #define VGA_BROWN_FOREGROUND         0x06
        #define VGA_GRAY_FOREGROUND          0x07
        #define VGA_DARKGRAY_FOREGROUND      0x08
        #define VGA_BRIGHTBLUE_FOREGROUND    0x09
        #define VGA_BRIGHTGREEN_FOREGROUND   0x0A
        #define VGA_BRIGHTCYAN_FOREGROUND    0x0B
        #define VGA_BRIGHTRED_FOREGROUND     0x0C
        #define VGA_BRIGHTMAGENTA_FOREGROUND 0x0D
        #define VGA_YELLOW_FOREGROUND        0x0E
        #define VGA_WHITE_FOREGROUND         0x0F

        #define VGA_BLACK_BACKGROUND         0x00
        #define VGA_BLUE_BACKGROUND          0x10
        #define VGA_GREEN_BACKGROUND         0x20
        #define VGA_CYAN_BACKGROUND          0x30
        #define VGA_RED_BACKGROUND           0x40
        #define VGA_MAGENTA_BACKGROUND       0x50
        #define VGA_BROWN_BACKGROUND         0x60
        #define VGA_GRAY_BACKGROUND          0x70
        #define VGA_DARKGRAY_BACKGROUND      0x80
        #define VGA_BRIGHTBLUE_BACKGROUND    0x90
        #define VGA_BRIGHTGREEN_BACKGROUND   0xA0
        #define VGA_BRIGHTCYAN_BACKGROUND    0xB0
        #define VGA_BRIGHTRED_BACKGROUND     0xC0
        #define VGA_BRIGHTMAGENTA_BACKGROUND 0xD0
        #define VGA_YELLOW_BACKGROUND        0xE0
        #define VGA_WHITE_BACKGROUND         0xF0

        #define VGA_TEXT_DEFAULT_COLOR       (VGA_WHITE_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_SUCCESS_COLOR       (VGA_BRIGHTGREEN_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_ERROR_COLOR         (VGA_BRIGHTRED_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_DEBUG_COLOR         (VGA_MAGENTA_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_WARN_COLOR          (VGA_YELLOW_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_INFO_COLOR          (VGA_CYAN_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_DATE_COLOR          (VGA_GRAY_FOREGROUND | VGA_BLACK_BACKGROUND)
        #define VGA_TEXT_PANIC_COLOR         (VGA_WHITE_FOREGROUND | VGA_BLUE_BACKGROUND)
        #define VGA_TEXT_PROMPT_COLOR        (VGA_BRIGHTMAGENTA_FOREGROUND | VGA_BLACK_BACKGROUND)
    #endif /* VGA_COLORS_VALUES_H_ */

/**
 * @brief Print a character on the screen at a certain coordinates using VGA.
 *
 * @param x             The coordinate X
 * @param y             The coordinate Y
 * @param c             The character
 * @param color         The color
 */
void
kvga_putc_at(uint8_t x, uint8_t y, char c, uint8_t color);

/**
 * @brief Refresh the cursor position using the new_position parameter.
 *
 * @param new_position          The new position
 */
void
kvga_set_cursor_position(point8_t new_position);

/**
 * @brief Set the cursor visibility to hidden.
 */
void
kvga_hide_cursor(void);

/**
 * @brief Set the cursor visibility to visible.
 */
void
kvga_show_cursor(void);

/**
 * @brief Do a scroll to the top. Using kmemmove.
 */
void
kvga_scroll_line(void);

/**
 * @brief Fill the VGA buffer to the c character and his color.
 *
 * @param c             The character
 * @param color         The color
 */
void
kvga_fill(char c, uint8_t color);

/**
 * @brief Print a character on the screen at a certain coordinates using physical VGA address.
 *
 * @param x             The coordinate X
 * @param y             The coordinate Y
 * @param c             The character
 * @param color         The color
 */
void
kvga_phys_putc_at(uint8_t x, uint8_t y, char c, uint8_t color);

/**
 * @brief Print a string on the screen at a certain coordinates using physical VGA address.
 *
 * @param x             The coordinate X
 * @param y             The coordinate Y
 * @param string        The string
 * @param color         The color
 */
void
kvga_phys_puts_at(uint8_t x, uint8_t y, const char *string, uint8_t color);

/**
 * @brief Fill the VGA buffer using physical address.
 *
 * @param c             The character
 * @param color         The color
 */
void
kvga_phys_fill(char c, uint8_t color);

/**
 * @brief Write data in the VGA buffer using a char device.
 *
 * @param node       The node of the file in the VFS
 * @param offset     The offset pointer in the file
 * @param buffer     The buffer where the data to write is in
 * @param len        The number of bytes to write
 *
 * @return The size of the number of bytes write in the file.
 */
size_t
kfs_vga_write(vfs_node_t *node, off_t *offset, const void *buffer, size_t len);

#endif /* ifndef DRIVER_VIDEO_VGA_H_ */
