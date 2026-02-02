/*
** DELOS PROJECT, 2026
** src/kernel/interruption/isr/isr_syscall
** File description:
** Single syscall entry point (int 0x80)
*/

#include <kernel/interruption/isr.h>
#include <kernel/scheduler/task.h>
#include <kernel/sys/syscall.h>
#include <defines.h>

/**
 * @brief ISR syscall.
 *
 * @param regs          The registers
 */
void
isr_syscall(isr_registers_t *regs)
{
    int32_t syscall_id = 0;

    if (regs == NULL) {
        return;
    }
    syscall_id = regs->_eax;
    regs->_eax = syscalls_array[syscall_id](regs->_ebx, regs->_ecx, regs->_edx, regs->_esi, regs->_edi);
    __builtin_unreachable();
}
