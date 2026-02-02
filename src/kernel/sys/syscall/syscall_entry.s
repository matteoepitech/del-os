; syscall.s

global __syscall__

__syscall__:
    mov esi, [esp + 4]
    mov edx, [esp + 8]
    mov ecx, [esp + 12]
    mov ebx, [esp + 16]
    mov eax, [esp + 20]
    int 0x80
    ret
