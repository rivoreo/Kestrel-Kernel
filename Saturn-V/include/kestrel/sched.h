/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _KESTREL_SCHED_H
#define _KESTREL_SCHED_H

//#define task_switch(N) __asm__("ljmp	(%0), 0\n" :: "g"(((N)+5)*8))
//#define task_switch(N) __asm__("	ljmp	*%0, $0\n" :: "i"((N)))

#define TASK_MAX 32

struct tss {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};

struct ktask {
	unsigned int index, flags;
	struct tss tss;
	//int esp;
};

struct ktaskctl {
	int running_count;
	int current;
	struct ktask tasks[TASK_MAX];
};

void init_sched(void);
void schedule(void);
struct ktask *make_task(void);

#endif
