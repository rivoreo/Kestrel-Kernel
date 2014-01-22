AS = as --32
CC = gcc -m32
LD = ld -melf_i386
#OBJCOPY = objcopy --only-section=.text
OBJCOPY = objcopy -R .note -R .comment -R .note.gnu.build-id
#OBJCOPY = objcopy

CFLAGS = -I../S-II -Os -fno-stack-protector -fno-builtin -nostdinc -Wall -Wmissing-prototypes -Wunused -Wshadow -Wpointer-arith -falign-jumps=1 -falign-loops=1 -falign-functions=1 -Wundef
LDFLAGS = -N -Ttext 8000 --build-id=none

start.com:	start
	$(OBJCOPY) -O binary $^ $@

start:	start.o
	$(LD) $(LDFLAGS) $^ -o $@

start.o:	start.s
	$(AS) $^ -o $@

start.s:	arch/$(ARCH)/start.S ../S-II/kernel-size.h
	$(CC) -E $(CFLAGS) arch/$(ARCH)/start.S -o $@

clean:
	rm -f start.s start.o start start.com
