/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>

int testint_command(int argc, char **argv) {
	extern int int20_handler();
	extern int int80_handler(int, ...);
/*
	if(argc < 2) {
		puts("Usage: testint <number>");
		return -1;
	}
	register int n = kernel_atoi(argv[1]);
	__asm__("int	%0\n" :: "r"(n):);
*/
	//__asm__("int	$0x13\n");
	//struct idtr *i = get_idtr();
	//kernel_printf("limits = %d\naddr = %d\n", i->limits, i->addr);
	struct idtr t;
	get_idtr(&t);
	kernel_printf("limits = %d\naddress = %d\n", t.limits, t.address);
	/*
	struct gd *gdaddr = (struct gd *)0x0;
	t.limits = 0x7ff;
	t.addr = gdaddr;
	set_idtr(&t);
	set_gate_handler(gdaddr + 0x20, int20_handler, 1 << 3, 0x8e);
	set_gate_handler(gdaddr + 0x80, int80_handler, 1 << 3, 0x8e);
	*/
	//if(argc > 1) __asm__("int	$0x20\n");
	kernel_puts("1");
	init_pic();
	kernel_puts("2");
	__asm__("sti\n");
	kernel_getchar();
	return 0;
}
