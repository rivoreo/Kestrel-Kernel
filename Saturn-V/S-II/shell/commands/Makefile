ifeq ($(CC),cc)
CC = gcc -m32
endif

CFLAGS = -I../../../include --include ../../../version.h --include ../../shared.h -O1 -Wall

COMMANDS = \
	argstest.o \
	date.o \
	doublefree.o \
	echo.o \
	graphics.o \
	help.o \
	malloctest.o \
	mem.o \
	panic.o \
	reboot.o \
	threadtest.o \
	uptime.o

commands.a:	$(COMMANDS)
	$(AR) r $@ $^

clean:
	rm -f *.o

distclean:	clean
	rm -f commands.a
