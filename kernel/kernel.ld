ENTRY(_start)


PHDRS {
  text PT_LOAD FLAGS (R | X);
  data PT_LOAD FLAGS (R | W);
}

SECTIONS
{
	. = 0x7E00;
	
	.text : {
		KEEP(*(.text.entry))   /* forces kernel_entry to be first, also prevents crashes due to misplaced kmain */
		*(.text)
	} :text

	.rodata : {
		*(.rodata)
	} :data

	.data : {
		*(.data)
	} :data 

	.bss : {
		*(.bss)
		*(COMMON)
	} :data

}