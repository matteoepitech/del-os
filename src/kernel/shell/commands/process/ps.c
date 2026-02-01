/*
** DELOS PROJECT, 2025
** src/kernel/shell/commands/ps
** File description:
** ps command source file
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/scheduler/process.h>
#include <kernel/shell/shell.h>
#include <utils/misc/print.h>
#include <defines.h>

/**
 * @brief Command for ps.
 *
 * @param argc          The number of argument
 * @param argv[]        The array of argument
 *
 * @return The final code of the operation.
 */
uint8_t
kshell_ps(UNUSED uint32_t argc, UNUSED char *argv[])
{
    uint32_t processes_amount = 0;
    process_t *process = NULL;
    task_t *task = NULL;

    if (ktask_head == NULL) {
        KPRINTF_WARN("ps: scheduler is empty");
        return OK_TRUE;
    }
    KPRINTF_INFO("%4s %6s %8s", "PID", "PPID", "NAME");
    task = ktask_head;
    while (task != NULL) {
        process = task->_process;
        if (process != NULL && process->_state != KPROCESS_DEAD && task == process->_main_thread) {
            KPRINTF_INFO("%4d %6d %8s", process->_pid, process->_ppid, process->_name);
            processes_amount++;
        }
        task = task->_next;
        if (task == ktask_head) {
            break;
        }
    }
    if (processes_amount == 0) {
        KPRINTF_WARN("ps: no process to display");
        return OK_TRUE;
    }
    return KO_FALSE;
}
