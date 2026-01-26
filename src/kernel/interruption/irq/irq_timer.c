/*
** DELOS PROJECT, 2025
** src/kernel/interruption/irq/irq_timer
** File description:
** IRQ timer (IRQ0)
*/

#include <kernel/scheduler/scheduler.h>
#include <kernel/interruption/isr.h>
#include <kernel/interruption/pic.h>
#include <kernel/interruption/pit.h>
#include <kernel/tty/tty.h>
#include <defines.h>

/**
 * @brief IRQ timer.
 * 
 * @param regs          The registers
 */
void
irq_timer(isr_registers_t *regs)
{
    static uint32_t sec_tick_count = 0;

    ticks_count++;
    if (++sec_tick_count % PIT_TARGET_FREQUENCY == 0) {
        sec_tick_count = 0;
        seconds_count++;
    }
    PIC_CALL_EOI();
    if (ticks_count % 100 == 0) {
        if (kscheduler_tick(regs) == OK_TRUE) {
            __builtin_unreachable();
        }
    }
    return;
}
