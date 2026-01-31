/*
** DELOS PROJECT, 2025
** src/kernel/interruption/idt
** File description:
** Interruption descriptor table source file
*/

#include <kernel/interruption/idt.h>
#include <kernel/interruption/isr.h>
#include <kernel/interruption/irq.h>
#include <utils/kstdlib/kmemory.h>
#include <kernel/misc/panic.h>
#include <kernel/tty/tty.h>
#include <defines.h>

/* ISR Declarations for stubs */
ISR_DECLARE_STUB(0);
ISR_DECLARE_STUB(1);
ISR_DECLARE_STUB(2);
ISR_DECLARE_STUB(3);
ISR_DECLARE_STUB(4);
ISR_DECLARE_STUB(5);
ISR_DECLARE_STUB(6);
ISR_DECLARE_STUB(7);
ISR_DECLARE_STUB(8);
ISR_DECLARE_STUB(9);
ISR_DECLARE_STUB(10);
ISR_DECLARE_STUB(11);
ISR_DECLARE_STUB(12);
ISR_DECLARE_STUB(13);
ISR_DECLARE_STUB(14);
ISR_DECLARE_STUB(15);
ISR_DECLARE_STUB(16);
ISR_DECLARE_STUB(17);
ISR_DECLARE_STUB(18);
ISR_DECLARE_STUB(19);
ISR_DECLARE_STUB(20);
ISR_DECLARE_STUB(21);
ISR_DECLARE_STUB(22);
ISR_DECLARE_STUB(23);
ISR_DECLARE_STUB(24);
ISR_DECLARE_STUB(25);
ISR_DECLARE_STUB(26);
ISR_DECLARE_STUB(27);
ISR_DECLARE_STUB(28);
ISR_DECLARE_STUB(29);
ISR_DECLARE_STUB(30);
ISR_DECLARE_STUB(31);
ISR_DECLARE_STUB(128);

/* ISR Declarations for handlers */
ISR_DECLARE_HANDLER(isr_div_zero);
ISR_DECLARE_HANDLER(isr_debug_exception);
ISR_DECLARE_HANDLER(isr_breakpoint);
ISR_DECLARE_HANDLER(isr_nmi);
ISR_DECLARE_HANDLER(isr_overflow);
ISR_DECLARE_HANDLER(isr_bound_range_exceeded);
ISR_DECLARE_HANDLER(isr_invalid_opcode);
ISR_DECLARE_HANDLER(isr_device_not_available);
ISR_DECLARE_HANDLER(isr_double_fault);
ISR_DECLARE_HANDLER(isr_coprocessor_segment_overrun);
ISR_DECLARE_HANDLER(isr_invalid_tss);
ISR_DECLARE_HANDLER(isr_segment_not_present);
ISR_DECLARE_HANDLER(isr_stack_segment_fault);
ISR_DECLARE_HANDLER(isr_general_protection_fault);
ISR_DECLARE_HANDLER(isr_page_fault);
ISR_DECLARE_HANDLER(isr_reserved_15);
ISR_DECLARE_HANDLER(isr_x87_floating_point);
ISR_DECLARE_HANDLER(isr_alignment_check);
ISR_DECLARE_HANDLER(isr_machine_check);
ISR_DECLARE_HANDLER(isr_simd_floating_point);
ISR_DECLARE_HANDLER(isr_virtualization_exception);
ISR_DECLARE_HANDLER(isr_control_protection_exception);
ISR_DECLARE_HANDLER(isr_reserved_22);
ISR_DECLARE_HANDLER(isr_reserved_23);
ISR_DECLARE_HANDLER(isr_reserved_24);
ISR_DECLARE_HANDLER(isr_reserved_25);
ISR_DECLARE_HANDLER(isr_reserved_26);
ISR_DECLARE_HANDLER(isr_reserved_27);
ISR_DECLARE_HANDLER(isr_reserved_28);
ISR_DECLARE_HANDLER(isr_reserved_29);
ISR_DECLARE_HANDLER(isr_security_exception);
ISR_DECLARE_HANDLER(isr_reserved_31);
ISR_DECLARE_HANDLER(isr_syscall);

/* IRQ Declarations for stubs */
IRQ_DECLARE_STUB(0);
IRQ_DECLARE_STUB(1);

/* IRQ Declarations for handlers */
IRQ_DECLARE_HANDLER(irq_timer);
IRQ_DECLARE_HANDLER(irq_keyboard_press);

/**
 * @brief The global variable containing every exceptions ISR (kernel space) for IDT.
 *        TODO: Setup every interruptions.
 */
static const idt_registration_t idt_kernel_exceptions_registration[] = {
    {0,  isr0,  isr_div_zero},
    {1,  isr1,  isr_debug_exception},
    //{2,  isr2,  isr_nmi},
    {3,  isr3,  isr_breakpoint},
    //{4,  isr4,  isr_overflow},
    //{5,  isr5,  isr_bound_range_exceeded},
    {6,  isr6,  isr_invalid_opcode},
    //{7,  isr7,  isr_device_not_available},
    //{8,  isr8,  isr_double_fault},
    //{9,  isr9,  isr_coprocessor_segment_overrun},
    //{10, isr10, isr_invalid_tss},
    //{11, isr11, isr_segment_not_present},
    //{12, isr12, isr_stack_segment_fault},
    //{13, isr13, isr_general_protection_fault},
    {14, isr14, isr_page_fault},
    //{15, isr15, isr_reserved_15},
    //{16, isr16, isr_x87_floating_point},
    //{17, isr17, isr_alignment_check},
    //{18, isr18, isr_machine_check},
    //{19, isr19, isr_simd_floating_point},
    //{20, isr20, isr_virtualization_exception},
    //{21, isr21, isr_control_protection_exception},
    //{22, isr22, isr_reserved_22},
    //{23, isr23, isr_reserved_23},
    //{24, isr24, isr_reserved_24},
    //{25, isr25, isr_reserved_25},
    //{26, isr26, isr_reserved_26},
    //{27, isr27, isr_reserved_27},
    //{28, isr28, isr_reserved_28},
    //{29, isr29, isr_reserved_29},
    //{30, isr30, isr_security_exception},
    //{31, isr31, isr_reserved_31},
};

/**
 * @brief The global variable containing every exceptions ISR (user space) for IDT.
 *        TODO: Setup every interruptions.
 */
static const idt_registration_t idt_user_exceptions_registration[] = {
    {128, isr128, isr_syscall},
};

/**
 * @brief The global variable containing every exceptions ISR/IRQ (hardware) for IDT.
 *        TODO: Setup every interruptions.
 */
static const idt_registration_t idt_hardware_irq_registration[] = {
    {32, irq0, irq_timer},
    {33, irq1, irq_keyboard_press},
};

/**
 * @brief Variable for the IDT pointer loaded in the CPU.
 *        This contains the size and address of the IDT.
 */
idt_ptr_t idt_ptr;

/**
 * @brief Variable for the IDT values loaded in the CPU.
 *        This contains all of the entries of the IDT.
 */
idt_entry_t idt[IDT_SIZE];

/**
 * @brief Register an IDT entry (handler + gate attributes).
 *
 * @param entry      The entry description (vector/stub/handler)
 * @param type_attr  Gate attributes (e.g., IDT_INT_GATE_KERNEL or _USER)
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
register_idt_entry(const idt_registration_t *entry, uint8_t type_attr)
{
    if (entry == NULL || entry->_stub == NULL || entry->_handler == NULL) {
        return KO_FALSE;
    }
    if (kidt_set_entry(entry->_vector, (uint32_t) entry->_stub, IDT_SEGMENT_CODE, type_attr) == KO_FALSE) {
        return KO_FALSE;
    }
    if (kisr_register_handler(entry->_vector, entry->_handler) == KO_FALSE) {
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Load all interruption service routine before the load in CPU.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
static bool32_t
idt_load_all_isr(void)
{
    uint32_t exception_count_kernel = sizeof(idt_kernel_exceptions_registration) / sizeof(idt_kernel_exceptions_registration[0]);
    uint32_t exception_count_user = sizeof(idt_user_exceptions_registration) / sizeof(idt_user_exceptions_registration[0]);
    uint32_t irq_count = sizeof(idt_hardware_irq_registration) / sizeof(idt_hardware_irq_registration[0]);

    for (uint32_t i = 0; i < exception_count_kernel; i += 1) {
        if (register_idt_entry(&idt_kernel_exceptions_registration[i], IDT_INT_GATE_KERNEL) == KO_FALSE) {
            return KO_FALSE;
        }
    }
    for (uint32_t i = 0; i < exception_count_user; i += 1) {
        if (register_idt_entry(&idt_user_exceptions_registration[i], IDT_INT_GATE_USER) == KO_FALSE) {
            return KO_FALSE;
        }
    }
    for (uint32_t i = 0; i < irq_count; i += 1) {
        if (register_idt_entry(&idt_hardware_irq_registration[i], IDT_INT_GATE_KERNEL) == KO_FALSE) {
            return KO_FALSE;
        }
    }
    return OK_TRUE;
}

/**
 * @brief Took the pointer passed as parameter and set his memory values.
 *
 * @param idt_ptr       The pointer to the future IDT
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kidt_create_ptr(idt_ptr_t *ptr)
{
    if (ptr == NULL) {
        return KO_FALSE;
    }
    ptr->_limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
    ptr->_base = (uint32_t) idt;
    kmemset(idt, 0, sizeof(idt));
    return OK_TRUE;
}

/**
 * @brief Load the idt_ptr in the CPU using lidt instruction.
 *
 * @param idt_ptr       The pointer to the future IDT
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kidt_load_cpu(idt_ptr_t *ptr)
{
    idt_ptr_t idt_ptr_check = {0};

    if (ptr == NULL) {
        return KO_FALSE;
    }
    if (ptr->_base == NULL || ptr->_limit == 0) {
        KPANIC("IDT's pointer is not created. Aborting...");
        return KO_FALSE;
    }
    if (idt_load_all_isr() == KO_FALSE) {
        return KO_FALSE;
    }
    __asm__ volatile ("lidt %0" : : "m"(*ptr));
    __asm__ volatile ("sidt %0" : "=m"(idt_ptr_check));
    if (idt_ptr_check._base == NULL) {
        KPANIC("Unable to load the IDT pointer.");
        return KO_FALSE;
    }
    return OK_TRUE;
}

/**
 * @brief Set an interruption in the IDT at the index.
 *
 * @param index                 The index in the IDT of this interruption
 * @param addr                  The address to jump at
 * @param selector              The segment to be at (segment code most of time)
 * @param type_attr             The attributes of this interruption
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kidt_set_entry(uint8_t index, uint32_t addr, uint16_t selector, uint8_t type_attr)
{
    if (addr == NULL) {
        return KO_FALSE;
    }
    idt[index]._offset_high = (addr & 0xFFFF0000) >> 16;
    idt[index]._offset_low = addr & 0x0000FFFF;
    idt[index]._selector = selector;
    idt[index]._type_attr = type_attr;
    idt[index]._zero = NULL;
    return OK_TRUE;
}
