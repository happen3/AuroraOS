ENTRY(_start)

SECTIONS
{
	. = 0x7E00;
	
	.text : {
		KEEP(*(.text.entry))   /* forces kernel_entry to be first, also prevents crashes due to misplaced kmain */
		*(.text)
	}

	.rodata : {
		*(.rodata)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
		*(COMMON)
	}

}
