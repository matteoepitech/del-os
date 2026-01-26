/*
** DELOS PROJECT, 2025
** include/kernel/interruption/isr
** File description:
** Interruption service routine header file
*/

#include <types.h>

#ifndef KERNEL_INTERRUTPION_ISR_H_
    #define KERNEL_INTERRUTPION_ISR_H_

    #ifndef ACTIVATE_TRAP_FLAG
        #define ACTIVATE_TRAP_FLAG(regs) regs->_eflags |= 0b100000000
    #endif /* ifndef ACTIVATE_TRAP_FLAG */

    #ifndef DEACTIVATE_TRAP_FLAG
        #define DEACTIVATE_TRAP_FLAG(regs) regs->_eflags &= ~0b100000000
    #endif /* ifndef DEACTIVATE_TRAP_FLAG */

    // This macro is used for declaring interruption service routine from stubs
    #ifndef ISR_DECLARE_STUB
        #define ISR_DECLARE_STUB(nb) extern void isr##nb(void)
    #endif /* ifndef ISR_DECLARE_STUB */

    // This macro is used for declaring interruption service routine easier
    #ifndef ISR_DECLARE_HANDLER
        #define ISR_DECLARE_HANDLER(name) extern void name(isr_registers_t *regs)
    #endif /* ifndef ISR_DECLARE_HANDLER */

/**
 * @brief Register structure passed by the CPU when interruption.
 * Order matches the stack layout produced by irq_stubs.s
 *        -...
 */
typedef struct isr_registers_s {
    uint32_t _gs;
    uint32_t _fs;
    uint32_t _es;
    uint32_t _ds;
    uint32_t _edi;
    uint32_t _esi;
    uint32_t _ebp;
    uint32_t _esp;
    uint32_t _ebx;
    uint32_t _edx;
    uint32_t _ecx;
    uint32_t _eax;
    uint32_t _int_no;
    uint32_t _err_code;
    uint32_t _eip;
    uint32_t _cs;
    uint32_t _eflags;
} isr_registers_t;

/**
 * @brief Typedef for the handler isr function type.
 */
typedef void (*isr_handler_t)(isr_registers_t *regs);

/**
 * @brief Register an handler of a ISR in the isr_handlers variable.
 *
 * @param index                 The index in the IDT and isr_handlers
 * @param func_handler          The function to jump at
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kisr_register_handler(uint8_t index, isr_handler_t func_handler);

/**
 * @brief Call the handler with the interruption number and his error code if provided.
 *
 * @param int_no                The interruption number
 * @param err_code              The error code if provided by the CPU
 */
__attribute__((used)) void
kisr_handler(isr_registers_t *regs);

#endif /* ifndef KERNEL_INTERRUTPION_ISR_H_ */
