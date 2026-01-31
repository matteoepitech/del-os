; isr_stubs.s
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global isr128

extern kisr_handler

; %1 = ISR number
; %2 = 0 if NOERR, 1 if ERR (CPU already push the err_code)
%macro ISR_COMMON 2
isr%1:
    cli

%if %2 == 0
    push 0              ; err_code if not pushed by the CPU
%endif

    push %1             ; int_no
    pusha               ; push EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            ; pointer to registers_t
    call kisr_handler
    add esp, 4          ; clean the pointer to registers_t

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 8          ; clean int_no and err_code
    iret
%endmacro

ISR_COMMON 0,0
ISR_COMMON 1,0
ISR_COMMON 2,0
ISR_COMMON 3,0
ISR_COMMON 4,0
ISR_COMMON 5,0
ISR_COMMON 6,0
ISR_COMMON 7,0
ISR_COMMON 8,1
ISR_COMMON 9,0
ISR_COMMON 10,1
ISR_COMMON 11,1
ISR_COMMON 12,1
ISR_COMMON 13,1
ISR_COMMON 14,1
ISR_COMMON 15,0
ISR_COMMON 16,0
ISR_COMMON 17,1
ISR_COMMON 18,0
ISR_COMMON 19,0
ISR_COMMON 20,0
ISR_COMMON 21,1
ISR_COMMON 22,0
ISR_COMMON 23,0
ISR_COMMON 24,0
ISR_COMMON 25,0
ISR_COMMON 26,0
ISR_COMMON 27,0
ISR_COMMON 28,0
ISR_COMMON 29,1
ISR_COMMON 30,1
ISR_COMMON 31,0
ISR_COMMON 128,0
