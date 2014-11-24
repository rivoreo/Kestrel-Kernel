/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/time.h>
#include <kestrel/errno.h>
#include <kestrel/sched.h>

static unsigned long int _uptime;

void init_timer() {
	kernel_puts("function: init_timer()");
	__asm__("	movb	$0x34, %%al\n"
		"	outb	%%al, $0x43\n"
		"	movb	$0x9c, %%al\n"
		"	outb	%%al, $0x40\n"
		"	movb	$0x2e, %%al\n"
		"	outb	%%al, $0x40\n"
		::: "%eax");
}

unsigned int test_number = 0;

void timer_interrupt_handler() {
	//static int i = 0;
	//stop();
	//kernel_puts("function: timer_interrupt_handler()");
	//if(i == 1000) kernel_printf("%d
	//kernel_putchar('.');
	//kernel_printf("%d ", test_number);
	//__asm__("cli\n");
	//kernel_panic("timer_interrupt_handler", ENOSYS);
	_uptime++;
	if(_uptime & 2) schedule();
}

unsigned long int get_uptime() {
	return _uptime;
}
