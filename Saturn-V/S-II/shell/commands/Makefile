ifeq ($(CC),cc)
CC = gcc -m32
endif

CFLAGS = -I../../../include --include ../../../version.h -O1 -Wall

COMMANDS = \
	panic.o

commands.a:	$(COMMANDS)
	$(AR) r $@ $^

clean:
	rm -f *.o

distclean:	clean
	rm -f commands.a
