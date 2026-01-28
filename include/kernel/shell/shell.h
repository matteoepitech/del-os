/*
** DELOS PROJECT, 2025
** include/kernel/shell/shell
** File description:
** Shell header file
*/

#include <kernel/shell/parser/arguments.h>
#include <defines.h>
#include <types.h>

#ifndef KERNEL_SHELL_H_
    #define KERNEL_SHELL_H_

    #ifndef KERNEL_SHELL_BUFFER_SIZE 
        #define KERNEL_SHELL_BUFFER_SIZE 512
    #endif /* ifndef KERNEL_SHELL_BUFFER_SIZE */

    #ifndef KERNEL_SHELL_PROMPT_BASE
        #define KERNEL_SHELL_PROMPT_BASE "$>"
    #endif /* ifndef KERNEL_SHELL_PROMPT_BASE */

/**
 * @brief The shell command structure definition.
 */
typedef struct shell_command_s {
    char *_command;
    uint8_t (*_func)(uint32_t argc, char *argv[]);
    char *_desc;
} shell_command_t;

/**
 * @brief All shell commands.
 */
extern const shell_command_t shell_commands[];

/**
 * @brief Execute the command
 *
 * @param argc               The ARGC
 * @param char *argv[16]     The ARGV
 *
 * @return The final exit code of the operation.
 */
uint8_t
kshell_execute_command(uint32_t argc, char *argv[SHELL_ARGV_MAX_COUNT]);

/**
 * @brief Start the shell and waiting for user prompt.
 *
 * @return OK_TRUE if worked KO_FALSE otherwise.
 */
bool32_t
kshell_start(void);

/**
 * @brief Start the shell using a task.
 *        Used on the scheduler to perform multi-task.
 */
void
kshell_start_task(void);

/*
 * -- COMMANDS --
 */

/**
 * @brief Command for reboot.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_reboot(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for help.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_help(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for clear.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_clear(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for uptime.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_uptime(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for timer.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_timer(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for shutdown.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_shutdown(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for panic_test.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_panic_test(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for repeat.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_repeat(uint32_t argc, char *argv[]);

/**
 * @brief Command for loadkeys.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_loadkeys(uint32_t argc, char *argv[]);

/**
 * @brief Command for debug.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_debug(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for dump_mem.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_mem_dump(uint32_t argc, char *argv[]);

/**
 * @brief Command for pmm_dump.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_pmm_dump(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for bitmap_dump.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_bitmap_dump(uint32_t argc, char *argv[]);

/**
 * @brief Command for heap_dump.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_heap_dump(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for ls.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_ls(uint32_t argc, char *argv[]);

/**
 * @brief Command for mkdir.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_mkdir(uint32_t argc, char *argv[]);

/**
 * @brief Command for touch.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_touch(uint32_t argc, char *argv[]);

/**
 * @brief Command for cat.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_cat(uint32_t argc, char *argv[]);

/**
 * @brief Command for unlink.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_unlink(uint32_t argc, char *argv[]);

/**
 * @brief Command for rmdir.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_rmdir(uint32_t argc, char *argv[]);

/**
 * @brief Command for pwd.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_pwd(UNUSED uint32_t argc, UNUSED char *argv[]);

/**
 * @brief Command for cd.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_cd(uint32_t argc, char *argv[]);

#endif /* ifndef KERNEL_SHELL_H_ */
