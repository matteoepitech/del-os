/*
** DELOS PROJECT, 2026
** include/kernel/scheduler/context
** File description:
** Context header file
*/

#include <kernel/scheduler/task.h>
#include <types.h>

#ifndef KERNEL_SCHEDULER_CONTEXT_H_
    #define KERNEL_SCHEDULER_CONTEXT_H_

    #define KERNEL_SCHEDULER_CONTEXT_TICKER 3

/*
 * @brief Structure for the CPU context switching.
 */
typedef struct cpu_context_s {
    uint32_t _edi;
    uint32_t _esi;
    uint32_t _ebp;
    uint32_t _ebx;
    uint32_t _edx;
    uint32_t _ecx;
    uint32_t _eax;
    uint32_t _eip;
    uint32_t _cs;
    uint32_t _eflags;
} cpu_context_t;

/**
 * @brief Enter the very first task context (no return).
 *
 * @param ctx    Pointer to the saved context to restore
 */
void
kcontext_enter_first(isr_registers_t *ctx); // Defined in the context.s

/**
 * @brief Switch from an ISR context to the given task context (no return).
 *
 * @param ctx    Pointer to the saved context to restore
 */
void
kcontext_switch_from_isr(isr_registers_t *ctx); // Defined in the context.s

#endif /* ifndef KERNEL_SCHEDULER_CONTEXT_H_ */
