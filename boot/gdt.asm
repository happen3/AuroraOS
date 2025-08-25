gdt_start:

    dq 0x0000000000000000     ; NULL descriptor (required)

    ; Code Segment Descriptor
    dq 0x00CF9A000000FFFF
    ; base = 0x00000000
    ; limit = 0xFFFFF (4GB with granularity)
    ; type = 0x9A (exec/read, code segment)
    ; flags = 0xC (32-bit, 4KB granularity)

    ; Data Segment Descriptor
    dq 0x00CF92000000FFFF
    ; base = 0x00000000
    ; limit = 0xFFFFF (4GB with granularity)
    ; type = 0x92 (read/write, data segment)
    ; flags = 0xC (32-bit, 4KB granularity)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; size (limit)
    dd gdt_start                 ; base linear address

CODE_SEL equ 0x08   ; 1st descriptor after null = code
DATA_SEL equ 0x10   ; 2nd descriptor after null = data
