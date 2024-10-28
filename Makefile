ISO=feur.iso
KERNEL_BIN=iso/boot/feur.bin
SRC_FILES=src/kernel.c src/vga.c src/term.c src/feurlib.c src/kinit.c src/exception.c src/handler.c src/shell.c
KERNEL_OBJ=$(SRC_FILES:src/%.c=obj/%.o)
MULTIBOOT_OBJ=obj/boot.o
GDT_OBJ=obj/kernel_initialization.o  # Ajouter la variable pour l'objet gdt.s

.PHONY: clean all make_iso compile re

#usb: make_iso
#	dd if=feur.iso of=/dev/sda bs=4M status=progress oflag=sync

all: make_iso
	qemu-system-i386 -cdrom $(ISO) -serial stdio

make_iso: $(KERNEL_BIN)
	grub-mkrescue --directory=i386-pc -o $(ISO) iso --modules="multiboot biosdisk"

$(KERNEL_BIN): $(KERNEL_OBJ) $(MULTIBOOT_OBJ) $(GDT_OBJ)  # Inclure gdt.o ici
	ld -m elf_i386 -T src/linker.ld -o $(KERNEL_BIN) -nostdlib $(MULTIBOOT_OBJ) $(KERNEL_OBJ) $(GDT_OBJ)

# Règle générique pour compiler les fichiers .c en fichiers .o
obj/%.o: src/%.c
	tinycc/i386-tcc -Iinclude/ -m32 -nostdlib -fno-builtin -fno-stack-protector -c $< -o $@

# Compilation du fichier assembleur boot.s
$(MULTIBOOT_OBJ): src/boot.s
	tinycc/i386-tcc -m32 -nostdlib -fno-builtin -fno-stack-protector -c src/boot.s -o $(MULTIBOOT_OBJ)

# Compilation du fichier assembleur gdt.s
$(GDT_OBJ): src/kernel_initialization.s
	nasm -f elf32 src/kernel_initialization.s -o $(GDT_OBJ)

clean:
	$(RM) $(ISO) $(KERNEL_OBJ) $(KERNEL_BIN) $(MULTIBOOT_OBJ) $(GDT_OBJ)

re: clean all
