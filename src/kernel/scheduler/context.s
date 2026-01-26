; context.s
; Context entry/switch helpers for the scheduler

[bits 32]

global kcontext_enter_first
global kcontext_switch_from_isr

; void kcontext_enter_first(isr_registers_t *ctx)
kcontext_enter_first:
    mov eax, [esp + 4]        ; eax = ctx*

    ; Load data segments from ctx
    mov bx, [eax + 12]        ; _ds
    mov ds, bx
    mov es, [eax + 8]         ; _es
    mov fs, [eax + 4]         ; _fs
    mov gs, [eax + 0]         ; _gs

    ; Load task stack pointer
    mov esp, [eax + 28]       ; _esp field (original esp at interrupt time)

    ; Push interrupt frame expected by iret (eflags, cs, eip)
    push dword [eax + 64]     ; _eflags
    push dword [eax + 60]     ; _cs
    push dword [eax + 56]     ; _eip

    sti                        ; allow interrupts for the task
    iret

; void kcontext_switch_from_isr(isr_registers_t *ctx)
; Assumes interrupts are disabled (in IRQ handler). Does not return.
kcontext_switch_from_isr:
    mov esi, [esp + 4]        ; esi = ctx*

    ; Load data segments from ctx
    mov bx, [esi + 12]        ; _ds
    mov ds, bx
    mov es, [esi + 8]         ; _es
    mov fs, [esi + 4]         ; _fs
    mov gs, [esi + 0]         ; _gs

    ; Switch to task stack
    mov esp, [esi + 28]       ; _esp

    ; Build iret frame on the new stack (eflags, cs, eip)
    push dword [esi + 64]
    push dword [esi + 60]
    push dword [esi + 56]

    ; Restore general-purpose registers
    mov edi, [esi + 16]
    mov ebp, [esi + 24]
    mov ebx, [esi + 32]
    mov edx, [esi + 36]
    mov ecx, [esi + 40]
    mov eax, [esi + 44]
    mov esi, [esi + 20]

    sti
    iret
