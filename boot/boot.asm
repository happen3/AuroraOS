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
		

; data
DATA db "[BOOTLOADER] enter;", 0

; boot signature
times 510-($-$$) db 0
dw 0xAA55
