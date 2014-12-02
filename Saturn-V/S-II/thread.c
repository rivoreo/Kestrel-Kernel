/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/thread.h>
#include <kestrel/sched.h>

ktask_t kernel_thread_create(ktask_func_t f, void *arg) {
	__asm__("cli\n");
	ktask_t r = make_task();
	r->tss.eip = (int)f;
 	r->tss.esp = (int)get_code_end() + 2048 * 2;	// XXX
 	unsigned char *p = (unsigned char *)r->tss.esp + 32, *pp = p;
	while(p-- > pp - 1024) *p = 0;
	((void **)(r->tss.esp))[1] = &arg;
	*((long int *)r->tss.esp) = 0xabcdef;
	kernel_printf("kernel_thread_create: r->tss.esp = 0x%lx\n", r->tss.esp);
	r->tss.ebp = r->tss.esp;
	//r->esp = r->tss.esp;
	//((void **)r->esp)[1] = f;
	//((long int *)r->esp)[2] = 0xabcdef;
	r->tss.es = 2 * 8;
	r->tss.ds = 2 * 8;
	r->tss.fs = 2 * 8;
	r->tss.gs = 2 * 8;
	r->tss.cs = 1 * 8;
	r->tss.ss = 2 * 8;
	__asm__("sti\n");
	return r;
}
