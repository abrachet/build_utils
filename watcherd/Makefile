
all: syscalls.s start.s watch.c
	gcc -ffreestanding -nostdlib watch.c syscalls.s start.s -o watcherd
