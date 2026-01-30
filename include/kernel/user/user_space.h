/*
** DELOS PROJECT, 2026
** include/kernel/user/user_space
** File description:
** User space header file
*/

#ifndef KERNEL_USER_SPACE_H_
    #define KERNEL_USER_SPACE_H_

    #define KPROCESS_USER_STACK_SIZE (16 * 1024)
    #define KPROCESS_USER_STACK_TOP  0xBFFFE000
    #define KPROCESS_USER_STACK_BOTTOM (KPROCESS_USER_STACK_TOP - KPROCESS_USER_STACK_SIZE)
    #define KPROCESS_USER_STACK_PAGE_COUNT (KPROCESS_USER_STACK_SIZE / KPMM_PAGE_SIZE)

#endif /* ifndef KERNEL_USER_SPACE_H_ */
