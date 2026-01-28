/*
** DELOS PROJECT, 2025
** src/kernel/shell/shell
** File description:
** Shell source file
*/

#include <kernel/shell/parser/autocomplete.h>
#include <kernel/shell/parser/arguments.h>
#include <utils/kstdlib/kstring.h>
#include <kernel/misc/keyboard.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>
#include <kernel/tty/tty.h>
#include <defines.h>

/**
 * @brief All shell commands.
 */
const shell_command_t shell_commands[] = {
    {"help", &kshell_help, "List available commands"},
    {"reboot", &kshell_reboot, "Reboot the system"},
    {"clear", &kshell_clear, "Clear the TTY screen"},
    {"uptime", &kshell_uptime, "Show system uptime"},
    {"timer", &kshell_timer, "Show timer information"},
    {"shutdown", &kshell_shutdown, "Shutdown the system"},
    {"panictest", &kshell_panic_test, "Trigger a kernel panic"},
    {"repeat", &kshell_repeat, "Repeat a command"},
    {"loadkeys", &kshell_loadkeys, "Load keyboard layout"},
    {"debug", &kshell_debug, "Debug using this command (dev only)"},
    {"memdump", &kshell_mem_dump, "Dump memory contents"},
    {"pmmdump", &kshell_pmm_dump, "Dump physical memory manager"},
    {"bitmapdump", &kshell_bitmap_dump, "Dump the bitmap values"},
    {"heapdump", &kshell_heap_dump, "Dump the informations about the heap (kmalloc)"},
    {"ls", &kshell_ls, "List files on a directory or root if none specified"},
    {"mkdir", &kshell_mkdir, "Create a directory on a specific path"},
    {"touch", &kshell_touch, "Create a file on a specific path"},
    {"cat", &kshell_cat, "Read the content of a file and print it"},
    {"unlink", &kshell_unlink, "Unlink a file (remove its entry and not its raw data)"},
    {"rmdir", &kshell_rmdir, "Remove a directory (when empty)"},
    {"pwd", &kshell_pwd, "Print the working directory"},
    {"cd", &kshell_cd, "Change directory"},
    {NULL, NULL, NULL}
};

/**
 * @brief Setup a command using the buffer and run the command.
 *
 * @param buffer                The buffer of the user prompt
 *
 * @return The final exit code of the operation.
 */
static uint8_t
setup_run_shell_command(char *buffer)
{
    char *argv[SHELL_ARGV_MAX_COUNT] = {0};
    uint32_t argc = 0;

    if (buffer == NULL || !*buffer) {
        return KO_FALSE;
    }
    argc = kshell_parse_get_argc(buffer);
    kshell_parse_formatting_buffer(buffer);
    kshell_parse_get_argv(buffer, argv);
    return kshell_execute_command(argc, argv);
}

/**
 * @brief Print the shell prompt. THAT IT.
 */
static
void print_shell_prompt(void)
{
    KPRINTF_DATE();
    ktty_puts(KERNEL_SHELL_PROMPT_BASE" ", VGA_TEXT_DEFAULT_COLOR);
}

/**
 * @brief Execute the command
 *
 * @param argc               The ARGC
 * @param char *argv[16]     The ARGV
 *
 * @return The final exit code of the operation.
 */
uint8_t
kshell_execute_command(uint32_t argc, char *argv[SHELL_ARGV_MAX_COUNT])
{
    if (argv == NULL || argv[0] == NULL) {
        return KO_FALSE;
    }
    for (uint32_t i = 0; shell_commands[i]._command != NULL; i++) {
        if (kstrcmp(argv[0], shell_commands[i]._command) != 0)
            continue;
        return shell_commands[i]._func(argc, argv);
    }
    KPRINTF_WARN("Command not found: %s", argv[0]);
    return OK_TRUE;
}

/**
 * @brief Start the shell and waiting for user prompt.
 *        This shell is the main process of the kernel and took the while.
 *
 * @return OK_TRUE if worked KO_FALSE otherwise.
 */
bool32_t
kshell_start(void)
{
    char buffer[KERNEL_SHELL_BUFFER_SIZE];
    uint32_t index = 0;

    KPRINTF_OK("shell: started...");
    print_shell_prompt();
    while (OK_TRUE) {
        uint8_t c = kkeyboard_getchar();

        switch (c) {
            case '\n':
                buffer[index] = '\0';
                ktty_puts("\n", VGA_TEXT_DEFAULT_COLOR);
                setup_run_shell_command(buffer);
                print_shell_prompt();
                index = 0;
                break;

            case '\b':
                if (index > 0) {
                    ktty_putc('\b', VGA_TEXT_DEFAULT_COLOR);
                    index--;
                }
                break;

            case '\t':
                kshell_parse_autocomplete(buffer, &index);
                break;

            default:
                if (index < KERNEL_SHELL_BUFFER_SIZE - 1) {
                    buffer[index] = c;
                    ktty_putc(c, VGA_TEXT_DEFAULT_COLOR);
                    index++;
                }
                break;
        }
    }
    return OK_TRUE;
}

/**
 * @brief Start the shell using a task.
 *        Used on the scheduler to perform multi-task.
 */
void
kshell_start_task(void)
{
    kshell_start();
    return;
}
