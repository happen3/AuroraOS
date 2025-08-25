[org 0x7C00]

boot:
	mov si, DATA
	call puts
	call boot_halt

boot_halt:
	jmp $

putc:
	mov ah, 0Eh
	mov bh, 00h
	mov bl, 0Fh
	int 10h
	ret

puts:
	pusha
	.loop:
		lodsb
		test al, al
		jz .end
		call putc
		jmp .loop
	.end:
		popa
		ret	
		

kernel_loader:
	pusha
	mov ax, 0
	mov es, ax
	mov dl, 0x80
	mov ah, 0x02
	mov al, 0x7F
	mov ch, 00h
	mov cl, 02
	mov bx, 0x7E00
	mov dh, 0
	int 0x13
	jc .fail
	.success:
		mov si, LOAD_SUCCESS
		call puts
		popa
		ret
	.fail:
		mov si, LOAD_ERROR
		call puts
		jmp $

kernel_jumper:
	cli ; disable interrupts for transition to protected mode
	lgdt [gdt_descriptor]

	mov eax, cr0 ; first step of enabling PE
	or eax, 1 ; second step of PE, set the PE (protection enable bit)
	mov cr0, eax ; final step of PE, push the new cr0

	; far jump to intermediate function
	jmp CODE_SEL:load_second_stage

[bits 32]
load_second_stage:
	; setup the stack and pass control to kernel
	mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x9FC00
	jmp 0x7E00 ; kernel code starts here

; data
DATA db "[BOOTLOADER] enter;", 0
LOAD_ERROR db "[BOOTLOADER] failed to load kernel;", 0
LOAD_SUCCESS db "[BOOTLOADER] successfully loaded kernel;"

%include "boot/gdt.asm"

; boot signature
times 510-($-$$) db 0
dw 0xAA55
