/*
** DELOS PROJECT, 2026
** src/kernel/interruption/isr/isr_syscall
** File description:
** Single syscall entry point (int 0x80)
*/

#include <kernel/interruption/isr.h>
#include <kernel/scheduler/task.h>
#include <defines.h>

/**
 * @brief ISR syscall.
 *
 * @param regs          The registers
 */
void
isr_syscall(isr_registers_t *regs)
{
    if (regs == NULL) {
        return;
    }
    ktask_exit();
    __builtin_unreachable();
}
