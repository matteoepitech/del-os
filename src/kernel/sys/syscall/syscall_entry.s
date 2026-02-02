; syscall.s

global __syscall__

__syscall__:
    mov eax, [esp + 4]
    mov ebx, [esp + 8]
    mov ecx, [esp + 12]
    mov edx, [esp + 16]
    mov esi, [esp + 20]
    mov edi, [esp + 24]
    int 0x80
    ret
