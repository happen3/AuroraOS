global default_isr_handler
global protection_error_handler
global invalid_op_handler
global double_fault_handler
global divide_by_zero_handler
global overflow_handler
global timer_irq
global keyb_irq

extern kputs_err
extern kputs
extern kputc
extern timer_tick
extern ps2kbd_handler

%include "kernel/cdecl.asm"
%define PIC_EOI 0x20 

put_frame_onto_page:
    mov esi, ebx
    mov edi, 0x9FD00
    mov ecx, 4
    rep movsd
    ret

default_isr_handler:
    pushad
    ; generic handler - use only for interrupts not handled by a more appropriate handler
    popad
    iret

protection_error_handler:
    pushad
    mov eax, esp
    call put_frame_onto_page
    popad
    pushad
    cli
    push protection_error_message
    call kputs_err
    pop eax
    jmp $
    popad
    iret

invalid_op_handler:
    pushad
    cli
    push invalid_op_message
    call kputs_err
    pop eax
    jmp $
    popad
    iret

double_fault_handler:
    pushad
    cli
    push double_fault_message
    call kputs_err
    pop eax
    jmp $
    popad
    iret

divide_by_zero_handler:
    pushad
    cli
    mov ebx, esp
    call put_frame_onto_page
    popad
    pushad
    push divide_error_message
    call kputs_err
    pop eax
    jmp $
    popad
    iret

overflow_handler:
    pushad
    cli
    push overflow_message
    call kputs_err
    pop eax
    jmp $
    popad
    iret

timer_irq:
    pushad
    call timer_tick
    mov al, PIC_EOI
    out 0x20, al
    popad
    iret

keyb_irq:
    pushad
    push edx
    push ecx
    push ebx
    push eax
    call ps2kbd_handler
    add esp, 16
    xor ax, ax
    .flush_loop:
        in al, 0x64 ; Read status
        test al, 1 ; Test for OBF
        jz .done
        in al, 0x60
        jmp .flush_loop
    .done:
        nop    
    mov al, PIC_EOI
    out 0x20, al
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