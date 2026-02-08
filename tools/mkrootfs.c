/*
** EPITECH PROJECT, 2026
** tools/mkrootfs
** File description:
** MKROOTFS source file
*/

// This file is a tool used to make a boot file system. Like a tar, it's just a continious img.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ROOTFS_NAME_MAX 64
#define ROOTFS_FILE 0
#define ROOTFS_DIR  1

/*
 * @brief Boot file system entry structure.
 *        - name = The name of the entry
 *        - size = The size of the entry
 *        - type = The type of the entry (VFS mode type bits)
 */
typedef struct rootfs_entry {
    char _name[ROOTFS_NAME_MAX];
    uint32_t _size;
    uint32_t _type;
} rootfs_entry_t;

/**
 * @brief Write an entry into the .img file.
 *
 * @param out    The file .img to write the entry to
 * @param name   The name of the entry
 * @param type   The type of the entry
 * @param data   The data pointer to the entry
 * @param size   The size of the entry
 */
static void
write_entry(FILE *out, const char *name, uint32_t type, const uint8_t *data, uint32_t size)
{
    rootfs_entry_t e = {0};

    strncpy(e._name, name, ROOTFS_NAME_MAX - 1);
    e._size = size;
    e._type = type;
    fwrite(&e, sizeof(e), 1, out);
    if (type == ROOTFS_FILE && size > 0) {
        fwrite(data, size, 1, out);
    }
}

/**
 * @brief Main entry of the tool MKROOTFS.
 *
 * @return The return value of the tool.
 */
int main(void)
{
    FILE *out = fopen("rootfs.img", "wb");

    if (out == NULL) {
        return 1;
    }
    write_entry(out, "/bin", ROOTFS_DIR, NULL, 0);
    FILE *f = fopen("rootfs/bin/README.md", "rb");
    if (f == NULL) {
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    uint8_t *buf = malloc(size);
    fread(buf, size, 1, f);
    fclose(f);
    write_entry(out, "/bin/README.md", ROOTFS_FILE, buf, size);
    free(buf);

    rootfs_entry_t end = {0};
    fwrite(&end, sizeof(end), 1, out);
    fclose(out);
    return 0;
}
