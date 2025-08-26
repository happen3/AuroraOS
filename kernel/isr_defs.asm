global default_isr_handler
global protection_error_handler
global invalid_op_handler
global double_fault_handler
global divide_by_zero_handler
global overflow_handler

extern kputs_err
extern kputs
extern kputc

%include "kernel/cdecl.asm"

default_isr_handler:
    pushad
    ; generic handler - use only for interrupts not handled by a more appropriate handmler
    popad
    iret

protection_error_handler:
    pushad
    push protection_error_message
    call kputs_err
    pop eax
    popad
    iret

invalid_op_handler:
    pushad
    push invalid_op_message
    call kputs_err
    pop eax
    popad
    iret
double_fault_handler:
    pushad
    push double_fault_message
    call kputs_err
    pop eax
    popad
    iret

divide_by_zero_handler:
    pushad
    push divide_error_message
    call kputs_err
    pop eax
    popad
    iret

overflow_handler:
    pushad
    push overflow_message
    call kputs_err
    pop eax
    popad
    iret

section .rodata
default_handler_message:
    db "DEFAULT HANDLER", 0
protection_error_message:
    db "General Protection Fault detected.", 0

divide_error_message:
    db "Divide By Zero error.", 0

invalid_op_message:
    db "Illegal opcode.", 0

double_fault_message:
    db "Double fault detected.", 0

overflow_message:
    db "Overflow was detected.", 0