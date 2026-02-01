/*
** DELOS PROJECT, 2026
** src/kernel/sys/syscall/syscall
** File description:
** Syscall source file
*/

#include <kernel/sys/syscall.h>

/*
 * @brief Array of syscalls. The position of this array determine the ID of the syscall in the EAX register
 */
const syscall_fn_t ksyscalls_array[] = {
    ksys_open,      // 1
    ksys_close,     // 2
    ksys_chdir,     // 3
    ksys_mkdir,     // 4
    ksys_rmdir,     // 5
    ksys_stat,      // 6
    ksys_fstat,     // 7
    ksys_unlink,    // 8
};
