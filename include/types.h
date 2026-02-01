/*
** DELOS PROJECT, 2025
** include/types
** File description:
** Types header file
*/

#ifndef TYPES_H_
    #define TYPES_H_

/**
 * @brief Unsigned char (1 byte)
 */
typedef unsigned char uint8_t;

/**
 * @brief Char (1 byte)
 */
typedef char int8_t;

/**
 * @brief Integer (4 bytes)
 */
typedef int int32_t;

/**
 * @brief Syscall argument type (register-sized signed int)
 */
typedef int32_t sysarg_t;

/**
 * @brief Unsigned integer (4 bytes)
 */
typedef unsigned int uint32_t;

/**
 * @brief Unsigned integer (8 bytes)
 */
typedef unsigned long long uint64_t;

/**
 * @brief Integer (8 bytes)
 */
typedef long long int64_t;

/**
 * @brief Unsigned integer (4 bytes)
 */
typedef uint32_t size_t;

/**
 * @brief Unsigned integer (4 bytes)
 *        Use in more specific case to precise this is an offset. (e.g. in VFS)
 */
typedef uint32_t off_t;

/**
 * @brief Signed integer (4 bytes)
 */
typedef int32_t fd_t;

/**
 * @brief Unsigned signed integer (4 bytes)
 */
typedef uint32_t pid_t;

/**
 * @brief Mode typedef unsigned integer 32 bits.
 */
typedef uint32_t mode_t;

/**
 * @brief UID typedef unsigned integer 32 bits.
 */
typedef uint32_t uid_t;

/**
 * @brief GID typedef unsigned integer 32 bits.
 */
typedef uint32_t gid_t;

/**
 * @brief Short (2 bytes)
 */
typedef short int16_t;

/**
 * @brief Unsigned short (2 bytes)
 */
typedef unsigned short uint16_t;


/**
 * @brief Unsigned char (1 byte)
 *        This type is used specially in structures or in arrays to save memory.
 *        Prefer using bool32_t if you work in stack, functions, condition for alignement.
 */
typedef uint8_t bool8_t;

/**
 * @brief Unsigned integer (4 bytes)
 */
typedef uint32_t bool32_t;

#endif /* ifndef TYPES_H_ */
