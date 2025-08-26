global default_isr_handler
extern kputs
extern kputc

%include "kernel/cdecl.asm"

default_isr_handler:
    pushad
    push 24
    push 0
    push default_handler_message
    mov bx, 3
    call cdecl_clear
    call kputs
    popad
    iret

section .rodata
default_handler_message:
    db "DEFAULT HANDLER", 0