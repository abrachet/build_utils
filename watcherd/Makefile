
all: syscalls.s start.s watch.c
	$(CC) -ffreestanding -nostdlib watcherd.c syscalls.s start.s -o watcherd
