/*
** EPITECH PROJECT, 2026
** tools/mkinitrd
** File description:
** MKINITRD source file
*/

// This file is a tool used to make a boot archive image.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define KINITRD_NAME_MAX 64
#define KINITRD_FILE 0
#define KINITRD_DIR  1

typedef struct initrd_entry {
    char _name[KINITRD_NAME_MAX];
    uint32_t _size;
    uint32_t _type;
} initrd_entry_t;

static void
write_entry(FILE *out, const char *name, uint32_t type, const uint8_t *data, uint32_t size)
{
    initrd_entry_t entry = {0};

    strncpy(entry._name, name, KINITRD_NAME_MAX - 1);
    entry._size = size;
    entry._type = type;
    fwrite(&entry, sizeof(entry), 1, out);
    if (type == KINITRD_FILE && size > 0) {
        fwrite(data, size, 1, out);
    }
}

int
main(void)
{
    FILE *out = fopen("initrd.img", "wb");
    FILE *file = NULL;
    uint8_t *buffer = NULL;
    long size = 0;
    initrd_entry_t end = {0};

    if (out == NULL) {
        return 1;
    }
    write_entry(out, "/bin", KINITRD_DIR, NULL, 0);
    file = fopen("initrd/bin/README.md", "rb");
    if (file == NULL) {
        fclose(out);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    buffer = malloc(size);
    if (buffer == NULL) {
        fclose(file);
        fclose(out);
        return 1;
    }
    fread(buffer, size, 1, file);
    fclose(file);
    write_entry(out, "/bin/README.md", KINITRD_FILE, buffer, size);
    free(buffer);
    fwrite(&end, sizeof(end), 1, out);
    fclose(out);
    return 0;
}
