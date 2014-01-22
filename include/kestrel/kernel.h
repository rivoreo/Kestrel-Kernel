/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _KESTREL_KERNEL_H
#define _KESTREL_KERNEL_H

#include <kestrel/types.h>

/* Memory map address range descriptor used by GET_MMAP_ENTRY. */
struct mmar_desc
{
  unsigned long desc_len;	/* Size of this descriptor. */
  unsigned long long addr;	/* Base address. */
  unsigned long long length;	/* Length in bytes. */
  unsigned long type;		/* Type of address range. */
} __attribute__ ((packed));

void stop(void) __attribute__((__noreturn__));
void panic(const char *, int) __attribute__((__noreturn__));

size_t kernel_strlen(const char *);
char *kernel_strcpy(char *, const char *);

int kernel_atoi(const char *);
long int kernel_atol(const char *);

int kernel_putchar(int);
int kernel_puts(const char *);
int kernel_printf(const char *, ...);

int gateA20(int);
int get_code_end(void);
int get_memsize(int);
int get_mmap_entry(struct mmar_desc *, int);
int getrtsecs(void);

extern unsigned long int saved_mem_upper;

#endif
