/*
** DELOS PROJECT, 2026
** include/kernel/sys/unistd
** File description:
** POSIX-like file descriptor constants
*/

#include <types.h>

#ifndef KERNEL_SYS_UNISTD_H_
    #define KERNEL_SYS_UNISTD_H_

    #ifndef KSTDIN_FILENO
        #define KSTDIN_FILENO ((fd_t) 0)
    #endif /* ifndef KSTDIN_FILENO */

    #ifndef KSTDOUT_FILENO
        #define KSTDOUT_FILENO ((fd_t) 1)
    #endif /* ifndef KSTDOUT_FILENO */

    #ifndef KSTDERR_FILENO
        #define KSTDERR_FILENO ((fd_t) 2)
    #endif /* ifndef KSTDERR_FILENO */

#endif /* ifndef KERNEL_SYS_UNISTD_H_ */
