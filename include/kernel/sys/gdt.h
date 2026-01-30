/*
** DELOS PROJECT, 2025
** include/kernel/system/gdt
** File description:
** GDT header file
*/

#include <defines.h>
#include <types.h>

#ifndef KERNEL_SYSTEM_GDT_H_
    #define KERNEL_SYSTEM_GDT_H_

    #ifndef KGDT_CODE_SELECTOR
        #define KGDT_CODE_SELECTOR 0x08
    #endif /* ifndef KGDT_CODE_SELECTOR */

    #ifndef KGDT_DATA_SELECTOR
        #define KGDT_DATA_SELECTOR 0x10
    #endif /* ifndef KGDT_DATA_SELECTOR */

    #ifndef KGDT_USER_CODE_SELECTOR
        #define KGDT_USER_CODE_SELECTOR 0x1B
    #endif /* ifndef KGDT_USER_CODE_SELECTOR */

    #ifndef KGDT_USER_DATA_SELECTOR
        #define KGDT_USER_DATA_SELECTOR 0x23
    #endif /* ifndef KGDT_USER_DATA_SELECTOR */

    #ifndef KGDT_TSS_SELECTOR
        #define KGDT_TSS_SELECTOR 0x28
    #endif /* ifndef KGDT_TSS_SELECTOR */

    #ifndef KGDT_RELOAD_PTR
        #define KGDT_RELOAD_PTR(ptr) do { \
            __asm__ volatile ( \
                "lgdt (%0)\n" \
                "mov $0x10, %%ax\n" \
                "mov %%ax, %%ds\n" \
                "mov %%ax, %%es\n" \
                "mov %%ax, %%fs\n" \
                "mov %%ax, %%gs\n" \
                "mov %%ax, %%ss\n" \
                "ljmp $0x08, $.flush_cs\n" \
                ".flush_cs:\n" \
                : \
                : "r" (ptr) \
                : "ax" \
            ); \
        } while (0)
    #endif /* ifndef KGDT_RELOAD_PTR */

/*
 * @brief The structure of a 32-bit TSS entry.
 */
typedef struct tss_entry_s {
    uint32_t _prev_tss;
    uint32_t _esp0;
    uint32_t _ss0;
    uint32_t _esp1;
    uint32_t _ss1;
    uint32_t _esp2;
    uint32_t _ss2;
    uint32_t _cr3;
    uint32_t _eip;
    uint32_t _eflags;
    uint32_t _eax;
    uint32_t _ecx;
    uint32_t _edx;
    uint32_t _ebx;
    uint32_t _esp;
    uint32_t _ebp;
    uint32_t _esi;
    uint32_t _edi;
    uint32_t _es;
    uint32_t _cs;
    uint32_t _ss;
    uint32_t _ds;
    uint32_t _fs;
    uint32_t _gs;
    uint32_t _ldt;
    uint16_t _trap;
    uint16_t _iomap_base;
} __attribute__((packed)) tss_entry_t;

/*
 * @brief The structure of a GDT entry (descriptor).
 *        Each entry describes a memory segment with its base address, size limit, access rights, and other properties.
 *        - limit_low = lower 16 bits of the segment limit (size - 1)
 *        - base_low = lower 16 bits of the segment base address
 *        - base_middle = middle 8 bits of the segment base address (bits 16-23)
 *        - access = access byte (present bit, privilege level, type, permissions)
 *        - granularity = upper 4 bits of limit + flags (granularity, size, long mode)
 *        - base_high = upper 8 bits of the segment base address (bits 24-31)
 * 
 *        NOTE: The structure is packed to match the exact GDT entry format required by x86 architecture.
 */
typedef struct gdt_entry_s {
    uint16_t _limit_low;
    uint16_t _base_low;
    uint8_t _base_middle;
    uint8_t _access;
    uint8_t _granularity;
    uint8_t _base_high;
} __attribute__((packed)) gdt_entry_t;

/*
 * @brief The structure of the GDT pointer (GDTR register content).
 *        This structure is loaded into the GDTR register using the LGDT instruction.
 *        - limit = size of the GDT in bytes - 1
 *        - base = linear address of the first byte of the GDT in memory
 *
 *        NOTE: The structure is packed to match the 6-byte format expected by LGDT.
 */
typedef struct gdt_ptr_s {
    uint16_t _limit;
    uint32_t _base;
} __attribute__((packed)) gdt_ptr_t;

/**
 * @brief Initialize a kernel-managed GDT mapped in the higher half.
 *
 * @return OK_TRUE if worked, KO_FALSE otherwise.
 */
bool32_t
kgdt_init(void);

/**
 * @brief Set the kernel stack on the GDT TSS.
 *
 * @param esp0   The esp0
 */
void
kgdt_set_kernel_stack(uint32_t esp0);

#endif /* ifndef KERNEL_SYSTEM_GDT_H_ */
