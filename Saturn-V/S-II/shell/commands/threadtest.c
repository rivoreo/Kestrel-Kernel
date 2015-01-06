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

__volatile__ int i = 10000;

void task1(void *a) {
}

void task2(void *a) {
	//*(long int *)0 = (long int)&i;
	while(1) {
		//__asm__("cli\n");
		//kernel_puts("Hello!!");
		//kernel_putchar('.');
		//__asm__("cli\n");
		//kernel_putchar('.');
		//__asm__("	ljmp	$40, $0\n");
		//__asm__("hlt\n");
	//	__volatile__ register int a = i;
	//	a = a - 2;
	//	i = a++;
	//	a++;
	//	i = a;
		i--;
		//i -= 2;
		//i = 1000;
	}
}

int threadtest_command(int argc, char **argv) {
	struct tss tss1, tss2;
	tss1.ldtr = 0;
	tss1.iomap = 0x40000000;
	tss2.ldtr = 0;
	tss2.iomap = 0x40000000;
	ktask_t t = kernel_thread_create(task2, NULL);
/*
	t->tss.es = 0x10;
	t->tss.ss = 0x10;
	t->tss.ds = 0x10;
	t->tss.fs = 0x10;
	t->tss.gs = 0x10;
*/
/*
	t->tss.backlink = 0;
	t->tss.ss0 = 0x10;
	t->tss.esp0 = 0x12000;
*/
	kernel_printf("t = 0x%lx\n", t);
	kernel_printf("t->index = %d\n", t->index);
	kernel_printf("t->tss.cs = %d\n", t->tss.cs);
	//kernel_getchar();
	//int i = 0;
	kernel_printf("&i = 0x%lx\n", &i);
	while(1) kernel_printf("%d\r", i += 10000);
	//__asm__ volatile("	ljmp	$48, $0\n");
	//int i = t->index;
	//__asm__ volatile("	ljmp	%0, $0\n" :: "g"(i));
	//task_switch(i);
	//__asm__("	ljmp	*%0, $0\n" :: "m"(i));
	//__asm__("	ljmp	%0\n" :: "m"(i));
	return 0;
}
