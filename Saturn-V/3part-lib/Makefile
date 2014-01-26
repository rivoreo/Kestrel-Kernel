AR = ar --target elf32-i386
ifeq ($(CC),cc)
CC = gcc -m32
endif

CFLAGS = -I../include --include ../S-II/shared.h -Os -fno-stack-protector -fno-builtin -Wall -Wunused -Wshadow -Wpointer-arith -falign-jumps=1 -falign-loops=1 -falign-functions=1 -Wundef

MULTI_STRING_OBJS = mstring.o

libmstr.a:	$(MULTI_STRING_OBJS)
	$(AR) -r $@ $^

clean:
	rm -f $(MULTI_STRING_OBJS) libmstr.a
