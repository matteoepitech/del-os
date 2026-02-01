/*
** DELOS PROJECT, 2026
** include/utils/asm/syscall
** File description:
** Syscall ASM header file
*/

#include <types.h>

#ifndef UTILS_ASM_SYSCALL_H_
    #define UTILS_ASM_SYSCALL_H_

/**
 * @brief Doing a syscall using the interruption 0x80.
 *
 * @param eax    Register..
 * @param ebx    Register..
 * @param ecx    Register..
 * @param edx    Register..
 * @param esi    Register..
 * @param edi    Register..
 * @param _      Register..
 *
 * @return The return of the syscall.
 */
static inline int32_t
syscall(int32_t eax, int32_t ebx, int32_t ecx, int32_t edx, int32_t esi, int32_t edi, int32_t _)
{
    (void) _;

    // AT&T syntax used by GCC
    int32_t ret;

    __asm__ volatile ("int $0x80" : "=a"(ret) : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx), "S"(esi), "D"(edi) : "memory");
    return ret;
}

#endif /* ifndef UTILS_ASM_SYSCALL_H_ */
