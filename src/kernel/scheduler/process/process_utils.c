/*
** DELOS PROJECT, 2026
** src/kernel/scheduler/process/process_manager
** File description:
** Process manager source file
*/

#include <kernel/scheduler/process.h>
#include <utils/kstdlib/kstring.h>

/**
 * @brief Kernel process init. It's the first task of the kernel.
 *
 * @param name          The name of the process.
 */
void
kprocess_set_name(char *name, process_t *process)
{
    kstrcpy(process->_name, name);
}
