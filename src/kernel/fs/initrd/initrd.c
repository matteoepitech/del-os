/*
** DELOS PROJECT, 2026
** src/kernel/fs/initrd/initrd
** File description:
** initrd unpack source file
*/

#include <utils/kstdlib/kmemory.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/fs/initrd/initrd.h>
#include <kernel/fs/vfs/vfs.h>

/**
 * @brief Get the VFS mode of an initrd entry.
 *
 * @param entry   The initrd entry
 *
 * @return The VFS mode bits of the entry.
 */
static mode_t
kinitrd_get_entry_mode(const initrd_entry_t *entry)
{
    if (entry == NULL) {
        return 0;
    }
    if (KVFS_STAT_TYPE(entry->_type) != 0) {
        return entry->_type;
    }
    return entry->_type == 1 ? KVFS_STAT_IFDIR : KVFS_STAT_IFREG;
}

/**
 * @brief Check if an initrd entry is the end marker.
 *
 * @param entry   The initrd entry to check
 *
 * @return OK_TRUE if the entry marks the end of the image, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_is_end_marker(const initrd_entry_t *entry)
{
    if (entry == NULL) {
        return KO_FALSE;
    }
    return entry->_name[0] == '\0' && entry->_size == 0 && entry->_type == 0;
}

/**
 * @brief Check that an initrd entry has a usable absolute path.
 *
 * @param entry   The initrd entry to validate
 *
 * @return OK_TRUE if the entry path can be used, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_has_valid_path(const initrd_entry_t *entry)
{
    if (entry == NULL || entry->_name[0] != '/') {
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Get the path part index to start iterating from.
 *
 * @param part_count   The number of split path parts
 * @param parts        The split path buffer
 *
 * @return The first index containing a real path component.
 */
static size_t
kinitrd_get_first_part_index(size_t part_count, char parts[KVFS_MAX_PATH_PARTS][KVFS_MAX_NAME_LEN])
{
    if (part_count > 0 && parts[0][0] == '/') {
        return 1;
    }
    return 0;
}

/**
 * @brief Create a child directory if it does not already exist.
 *
 * @param parent   The parent node
 * @param name     The child directory name
 *
 * @return The looked-up child node, or NULL on error.
 */
static vfs_node_t *
kinitrd_get_or_create_directory(vfs_node_t *parent, const char *name)
{
    vfs_node_t *child = NULL;

    if (parent == NULL || name == NULL) {
        return NULL;
    }
    child = kvfs_lookup(parent, name);
    if (child != NULL) {
        return child;
    }
    if (kvfs_mkdir(parent, name, 0755) == KO_FALSE) {
        return NULL;
    }
    return kvfs_lookup(parent, name);
}

/**
 * @brief Resolve a directory path and create missing components.
 *
 * @param root        The writable root node
 * @param parts       The split path buffer
 * @param part_count  The amount of parts to resolve
 *
 * @return The resolved directory node, or NULL on error.
 */
static vfs_node_t *
kinitrd_ensure_directory(vfs_node_t *root, char parts[KVFS_MAX_PATH_PARTS][KVFS_MAX_NAME_LEN], size_t part_count)
{
    vfs_node_t *current = NULL;
    vfs_node_t *next = NULL;
    size_t start_index = 0;

    if (root == NULL) {
        return NULL;
    }
    current = root;
    start_index = kinitrd_get_first_part_index(part_count, parts);
    for (size_t i = start_index; i < part_count; i++) {
        next = kinitrd_get_or_create_directory(current, parts[i]);
        if (next == NULL) {
            if (current != root) {
                kvfs_close(current);
            }
            return NULL;
        }
        if (current != root) {
            kvfs_close(current);
        }
        current = next;
    }
    return current;
}

/**
 * @brief Split an initrd entry path into VFS path components.
 *
 * @param entry       The initrd entry
 * @param out_parts   The destination split path buffer
 *
 * @return The number of path parts found.
 */
static size_t
kinitrd_split_entry_path(const initrd_entry_t *entry, char out_parts[KVFS_MAX_PATH_PARTS][KVFS_MAX_NAME_LEN])
{
    if (kinitrd_has_valid_path(entry) == KO_FALSE) {
        return 0;
    }
    return kvfs_split_path(entry->_name, out_parts);
}

/**
 * @brief Create a directory entry from an initrd entry.
 *
 * @param root    The writable root node
 * @param entry   The initrd entry to unpack
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_unpack_directory(vfs_node_t *root, const initrd_entry_t *entry)
{
    char parts[KVFS_MAX_PATH_PARTS][KVFS_MAX_NAME_LEN] = {0};
    size_t part_count = 0;
    vfs_node_t *dir = NULL;

    part_count = kinitrd_split_entry_path(entry, parts);
    if (part_count == 0) {
        return KO_FALSE;
    }
    dir = kinitrd_ensure_directory(root, parts, part_count);
    if (dir == NULL) {
        return KO_FALSE;
    }
    if (dir != root) {
        kvfs_close(dir);
    }
    return OK_TRUE;
}

/**
 * @brief Resolve the parent directory of a file entry.
 *
 * @param root        The writable root node
 * @param entry       The initrd entry
 * @param out_name    The final file name
 *
 * @return The resolved parent directory, or NULL on error.
 */
static vfs_node_t *
kinitrd_resolve_file_parent(vfs_node_t *root, const initrd_entry_t *entry, char out_name[KVFS_MAX_NAME_LEN])
{
    char parts[KVFS_MAX_PATH_PARTS][KVFS_MAX_NAME_LEN] = {0};
    size_t part_count = 0;

    part_count = kinitrd_split_entry_path(entry, parts);
    if (part_count < 2) {
        return NULL;
    }
    kstrcpy(out_name, parts[part_count - 1]);
    return kinitrd_ensure_directory(root, parts, part_count - 1);
}

/**
 * @brief Open a file if it exists, or create it and open it.
 *
 * @param parent   The parent directory node
 * @param name     The file name
 *
 * @return The file node, or NULL on error.
 */
static vfs_node_t *
kinitrd_open_or_create_file(vfs_node_t *parent, const char *name)
{
    vfs_node_t *file = NULL;

    if (parent == NULL || name == NULL) {
        return NULL;
    }
    file = kvfs_lookup(parent, name);
    if (file != NULL) {
        return file;
    }
    if (kvfs_create(parent, name, 0644 | KVFS_STAT_IFREG) == KO_FALSE) {
        return NULL;
    }
    return kvfs_lookup(parent, name);
}

/**
 * @brief Write the raw data of an initrd file entry into a VFS file node.
 *
 * @param file       The VFS file node to write to
 * @param entry      The initrd entry providing the size
 * @param data_ptr   The raw file data pointer
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_write_file_data(vfs_node_t *file, const initrd_entry_t *entry, const uint8_t *data_ptr)
{
    off_t offset = 0;

    if (file == NULL || entry == NULL || (entry->_size > 0 && data_ptr == NULL)) {
        return KO_FALSE;
    }
    if (entry->_size == 0) {
        return OK_TRUE;
    }
    if (kvfs_write(file, &offset, data_ptr, entry->_size) != entry->_size) {
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Create a file entry from an initrd entry.
 *
 * @param root       The writable root node
 * @param entry      The initrd entry to unpack
 * @param data_ptr   The raw file data pointer
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_unpack_file(vfs_node_t *root, const initrd_entry_t *entry, const uint8_t *data_ptr)
{
    char file_name[KVFS_MAX_NAME_LEN] = {0};
    vfs_node_t *parent = NULL;
    vfs_node_t *file = NULL;

    if (root == NULL) {
        return KO_FALSE;
    }
    parent = kinitrd_resolve_file_parent(root, entry, file_name);
    if (parent == NULL) {
        return KO_FALSE;
    }
    file = kinitrd_open_or_create_file(parent, file_name);
    if (file == NULL) {
        if (parent != root) {
            kvfs_close(parent);
        }
        return KO_FALSE;
    }
    if (kinitrd_write_file_data(file, entry, data_ptr) == KO_FALSE) {
        kvfs_close(file);
        if (parent != root) {
            kvfs_close(parent);
        }
        return KO_FALSE;
    }
    kvfs_close(file);
    if (parent != root) {
        kvfs_close(parent);
    }
    return OK_TRUE;
}

/**
 * @brief Unpack a single initrd entry into the VFS tree.
 *
 * @param root       The writable root node
 * @param entry      The initrd entry to unpack
 * @param data_ptr   The raw file data pointer of the entry
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_unpack_entry(vfs_node_t *root, const initrd_entry_t *entry, const uint8_t *data_ptr)
{
    mode_t mode = 0;

    if (root == NULL || entry == NULL) {
        return KO_FALSE;
    }
    mode = kinitrd_get_entry_mode(entry);
    if (KVFS_STAT_ISDIR(mode) != KO_FALSE) {
        return kinitrd_unpack_directory(root, entry);
    }
    if (KVFS_STAT_ISREG(mode) != KO_FALSE) {
        return kinitrd_unpack_file(root, entry, data_ptr);
    }
    return KO_FALSE;
}

/**
 * @brief Check whether enough bytes remain to read an initrd entry payload.
 *
 * @param cursor   The current cursor in the image
 * @param end      The end pointer of the image
 * @param size     The requested payload size
 *
 * @return OK_TRUE if the payload fits in the image, KO_FALSE otherwise.
 */
static bool32_t
kinitrd_has_remaining_data(const uint8_t *cursor, const uint8_t *end, size_t size)
{
    if (cursor == NULL || end == NULL || cursor > end) {
        return KO_FALSE;
    }
    if ((size_t) (end - cursor) < size) {
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Check if an initrd image is at least large enough to contain one entry.
 *
 * @param image_size   The size of the embedded image
 *
 * @return OK_TRUE if the image can contain entries, KO_FALSE otherwise.
 */
bool32_t
kinitrd_is_size_valid(size_t image_size)
{
    return image_size >= sizeof(initrd_entry_t) ? OK_TRUE : KO_FALSE;
}

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
kinitrd_unpack(vfs_node_t *root, const void *image, size_t image_size)
{
    const uint8_t *cursor = NULL;
    const uint8_t *end = NULL;
    const initrd_entry_t *entry = NULL;

    if (root == NULL || image == NULL || kinitrd_is_size_valid(image_size) == KO_FALSE) {
        return KO_FALSE;
    }
    cursor = image;
    end = cursor + image_size;
    while ((size_t) (end - cursor) >= sizeof(initrd_entry_t)) {
        entry = (const initrd_entry_t *) cursor;
        if (kinitrd_is_end_marker(entry) != KO_FALSE) {
            return OK_TRUE;
        }
        cursor += sizeof(initrd_entry_t);
        if (kinitrd_has_remaining_data(cursor, end, entry->_size) == KO_FALSE) {
            return KO_FALSE;
        }
        if (kinitrd_unpack_entry(root, entry, cursor) == KO_FALSE) {
            return KO_FALSE;
        }
        cursor += entry->_size;
    }
    return KO_FALSE;
}
