AS = "nasm"
CC = "i686-elf-gcc" -ffreestanding -g
LD = "i686-elf-ld"
OBJCOPY = "i686-elf-objcopy"

all: build/boot.bin build/kernel.elf.o build/kstring.o build/idt.o\
	 build/isr_defs.o build/mem.o build/vga_txt.o build/kernel.elf build/kernel.bin build/bootimg.img\
	 arch/x86/boot.img arch/x86/kernel.elf done

build/boot.bin: boot/boot.asm
	mkdir -p build
	@echo "Using assembler: $(AS)"
	@echo "* AS: boot/boot.asm"
	@$(AS) boot/boot.asm -f bin -o build/boot.bin
	
build/kernel.elf.o: kernel/kernel.c
	@echo "Building kernel as ELF32"
	@echo "* CC: kernel/kernel.c"
	@$(CC) -c $< -o $@

build/kstring.o: kernel/kstring.c
	@echo "* CC: kernel/kstring.c"
	@$(CC) -c $< -o $@	

build/idt.o: kernel/idt.c
	@echo "* CC: kernel/idt.c"
	@$(CC) -c $< -o $@

build/isr_defs.o: kernel/isr_defs.asm
	@echo "* AS: kernel/isr_defs.asm"
	@$(AS) kernel/isr_defs.asm -f elf32 -o build/isr_defs.o

build/io.o: kernel/io.c
	@echo "* CC: kernel/io.c"
	@$(CC) -c $< -o $@

build/pic.o: kernel/pic.c
	@echo "* CC: kernel/pic.c"
	@$(CC) -c $< -o $@

build/mem.o: kernel/mem.c
	@echo "* CC: kernel/mem.c"
	@$(CC) -c $< -o $@

build/vga_txt.o: kernel/vga_txt.c
	@echo "* CC: kernel/vga_txt.c"
	@$(CC) -c $< -o $@

build/kernel.elf: build/kernel.elf.o build/kstring.o build/isr_defs.o build/idt.o build/io.o build/pic.o build/mem.o\
				  build/vga_txt.o
	@echo "* LD: build/kernel.elf.o"
	@$(LD) -T kernel/kernel.ld -o $@ $^
	@echo "Compilation finished"

build/kernel.bin: build/kernel.elf
	@echo "Processing kernel.elf"
	@echo "* OBJCOPY: build/kernel.elf"
	@$(OBJCOPY) -O binary $< $@
	@echo "Finished objcopy process on kernel"

build/bootimg.img: build/boot.bin build/kernel.bin
	@echo "Writing bootable image"
	@echo "* TRUNCATE: build/bootimg.img"
	@truncate build/bootimg.img -s 1M
	@echo "* DD: build/bootimg.img using built files"
	@dd if=build/boot.bin of=build/bootimg.img conv=notrunc seek=0 > /dev/null 2> /dev/null
	@dd if=build/kernel.bin of=build/bootimg.img conv=notrunc seek=1 > /dev/null 2> /dev/null
	@echo "Done building boot image"

arch/x86/boot.img: build/bootimg.img
	@mkdir -p arch/x86
	@cp $< $@

arch/x86/kernel.elf: build/kernel.elf
	@mkdir -p arch/x86
	@cp $< $@

qemu_test: build/bootimg.img
	@qemu-system-x86_64 -hda build/bootimg.img -serial stdio

done:
	@echo "Find built image at: arch/x86/boot.img"
	@echo "Find kernel at     : arch/x86/kernel.elf"

clean:
	rm -rf build
	rm -rf arch
