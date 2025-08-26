global default_isr_handler
global protection_error_handler
global invalid_op_handler
global double_fault_handler
global divide_by_zero_handler

extern kputs
extern kputc

%include "kernel/cdecl.asm"

default_isr_handler:
    pushad
    push 24
    push 0
    push default_handler_message
    call kputs
    pop eax
    pop eax
    pop eax
    popad
    iret

protection_error_handler:
    pushad
    push 24
    push 0
    push protection_error_message
    call kputs
    pop eax
    pop eax
    pop eax
    popad
    iret

invalid_op_handler:
    iret

double_fault_handler:
    iret

divide_by_zero_handler:
    iret

section .rodata
default_handler_message:
    db "DEFAULT HANDLER", 0
protection_error_message:
    db "General Protection Fault detected.", 0