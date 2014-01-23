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

/* Values for the second argument to access.
   These may be OR'd together.  */
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */

/* Memory map address range descriptor used by GET_MMAP_ENTRY. */
struct mmar_desc
{
  unsigned long desc_len;	/* Size of this descriptor. */
  unsigned long long addr;	/* Base address. */
  unsigned long long length;	/* Length in bytes. */
  unsigned long type;		/* Type of address range. */
} __attribute__ ((packed));

void stop(void) __attribute__((__noreturn__));
void panic(const char *) __attribute__((__noreturn__));
void kernel_panic(const char *, int) __attribute__((__noreturn__));

size_t get_bios_time(void);
size_t kernel_strlen(const char *);
char *kernel_strcpy(char *, const char *);
void *kernel_memcpy(void *, const void *, size_t);

int kernel_atoi(const char *);
long int kernel_atol(const char *);

int kernel_access(const char *, int);
int kernel_open(const char *, int);
int kernel_close(int);

int kernel_putchar(int);
int kernel_puts(const char *);
int kernel_printf(const char *, ...);
size_t kernel_get_bios_time(void);

char *convert_to_ascii(char *, int, ...);

int gateA20(int);
int get_code_end(void);
int get_memsize(int);
int get_mmap_entry(struct mmar_desc *, int);
int getrtsecs(void);

void init_memory(void);

extern unsigned long int saved_mem_upper;
extern unsigned long int saved_mem_lower;
extern unsigned long int saved_mmap_addr;
extern unsigned long int saved_mmap_length;


#endif
