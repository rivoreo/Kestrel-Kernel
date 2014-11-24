/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/sched.h>
#include <kestrel/thread.h>

#include <kestrel/time.h>

static struct sd *task_gdt;
static struct ktaskctl kernel_thread_list;

ktask_t make_task() {
	int i;
	for(i=0; i<TASK_MAX; i++) {
		ktask_t t = kernel_thread_list.tasks + i;
		if(t->flags == 0) {
			kernel_thread_list.running_count++;
			t->flags = 1;
			t->tss.eflags = 0x00000202;
			t->tss.eax = 0;
			t->tss.ecx = 0;
			t->tss.edx = 0;
			t->tss.ebx = 0;
			t->tss.ebp = 0;
			t->tss.esi = 0;
			t->tss.edi = 0;
			t->tss.es = 0;
			t->tss.ds = 0;
			t->tss.fs = 0;
			t->tss.gs = 0;
			t->tss.ldtr = 0;
			t->tss.iomap = 0x40000000;
			return t;
		}
	}
	return NULL;
}

void init_sched() {
	kernel_puts("function: init_sched()");
	//set_int20_handler();
	struct td d;
	get_gdtr(&d);
	kernel_printf("limit: %d\naddress: 0x%lx\n", d.limit, d.address);
	int i;
	__asm__("	cli\n");
	struct sd *main_task_gdt = (struct sd *)d.address + 1;
	task_gdt = (struct sd *)d.address + 5;
	//set_segment_tss(main_task_gdt, &kernel_thread_list.tasks->tss);
	for(i=0; i<TASK_MAX; i++) {
		ktask_t t = kernel_thread_list.tasks + i;
		t->flags = 0;
		t->index = (5 + i) * sizeof(struct sd);
		/*if(i)*/ set_segment_descriptor(task_gdt + i, 103, &t->tss, 0x0089);
	}
	ktask_t r = make_task();		// Make the original kernel thread to thread 0
	// TODO: Assert here
	r->flags = 2;
	//r->index = 1 * sizeof(struct sd);
	//kernel_thread_list.running_count = 1;
	kernel_thread_list.current = 0;
	//r->index = 6*8;
	__asm__("	ltr	(%0)\n" :: "r"(&r->index));
	//__asm__("	sti\n");
}

void task_switch(volatile int i) {
/*
	struct {
		short int cs;
		long int eip;
	} __attribute__ ((packed)) tmp = { i, 0 };
*/
	__asm__("	ljmp	*%0\n" :: "m"(*(((char *)&i) - 4)));
	//__asm__("	movl	%esp, %0\n" :: "=r"(
}

void schedule() {
/*
	//int uptime = get_uptime();
	//if(uptime < 1010 && uptime > 990) kernel_printf("uptime: %u\n", uptime);

	if(get_uptime() == 999) {		// DEBUGGING
		int i = 48;
		kernel_putchar('!');
		task_switch(i);
		//__asm__("ljmp	$48, $0\n");
	}
	return;
*/
	if(kernel_thread_list.running_count < 2) return;
	//__asm__("	movl	%%esp, %0\n" : "=r"(kernel_thread_list.tasks[kernel_thread_list.current].esp));
	if(++kernel_thread_list.current == kernel_thread_list.running_count) {
		kernel_thread_list.current = 0;
	}
	int i = kernel_thread_list.tasks[kernel_thread_list.current].index;
	task_switch(i);
	//task_switch(kernel_thread_list.current);	// DEPRECATED
	__asm__("	sti\n");
	//__asm__("	movl	%0, %%esp\n" :: "r"(kernel_thread_list.tasks[kernel_thread_list.current].esp));
}
