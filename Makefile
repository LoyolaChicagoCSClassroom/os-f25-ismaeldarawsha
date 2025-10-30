# Set up the cross-compiler
CC = i386-elf-gcc
LD = i386-elf-ld
NASM = nasm

# Compiler flags
CFLAGS = -ffreestanding -m32 -Wall -Wextra -nostdlib -g
LDFLAGS = -melf_i386 -T link.ld -nostdlib

# Files
OBJ = kernel.o paging.o
KERNEL_IMG = kernel.bin

# Default target to build the kernel image
all: $(KERNEL_IMG)

$(KERNEL_IMG): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

kernel.o: kernel.c paging.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

paging.o: paging.c paging.h
	$(CC) $(CFLAGS) -c paging.c -o paging.o

# Clean up build files
clean:
	rm -f $(OBJ) $(KERNEL_IMG)

# Run the kernel with QEMU
run: $(KERNEL_IMG)
	qemu-system-i386 -kernel $(KERNEL_IMG) -monitor stdio

debug: $(KERNEL_IMG)
	screen -S qemu -d -m qemu-system-i386 -S -s -hda rootfs.img -monitor stdio
	TERM=xterm i386-unknown-elf-gdb -x gdb_os.txt && killall qemu-system-i386
