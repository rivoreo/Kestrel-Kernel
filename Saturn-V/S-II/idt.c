/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>

static struct gd *idt;

void get_idtr(struct idtr *p) {
	__asm__("	sidt	(%0)\n" :: "p" (p));
}

void set_idtr(const struct idtr *p) {
	__asm__("	cli\n");
	__asm__("	lidt	(%0)\n" :: "p" (p));
	__asm__("	sti\n");
}

void set_gate_handler(struct gd *gd, int (*handler)(), int selector, int ar) {
	__asm__("	cli\n");
	gd->offset_low = (int)handler & 0xffff;
	gd->selector = selector;
	gd->count = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high = ((int)handler >> 16) & 0xffff;
	__asm__("	sti\n");
}

void init_idt() {
	kernel_puts("function: init_idt()");
	idt = (struct gd *)0x0;
	struct idtr t = {
		.limits = 0x7ff,
		.address = idt
	};
	set_idtr(&t);
	set_gate_handler(idt + 0x20, int20_handler, 1 << 3, 0x8e);
	set_gate_handler(idt + 0x21, int21_handler, 1 << 3, 0x8e);
	set_gate_handler(idt + 0x27, int27_handler, 1 << 3, 0x8e);
	set_gate_handler(idt + 0x80, int80_handler, 1 << 3, 0x8e);
}
