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

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Values for the second argument to access.
   These may be OR'd together.  */
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */

#define kernel_alloc kernel_malloc

/* Memory map address range descriptor used by GET_MMAP_ENTRY. */
struct mmar_desc {
	unsigned long int desc_len;	/* Size of this descriptor. */
	unsigned long long int addr;	/* Base address. */
	unsigned long long int length;	/* Length in bytes. */
	unsigned long int type;		/* Type of address range. */
} __attribute__ ((packed));

struct mem_control_block {
	unsigned int is_available;
	size_t size;
};

struct sd {
	short int limit_low, base_low;
	char base_mid, access_right, limit_high, base_high;
} __attribute__((packed));

struct gd {
	short int offset_low, selector;
	char count, access_right;
	short int offset_high;
} __attribute__((packed));

struct td {
	short int limit;
	void *address;
} __attribute__((packed));

void stop(void) __attribute__((__noreturn__));
void panic(const char *) __attribute__((__noreturn__));
void kernel_panic(const char *, int) __attribute__((__noreturn__));
void kernel_reboot(void) __attribute__((__noreturn__));

size_t kernel_strlen(const char *);
int kernel_strcmp(const char *, const char *);
char *kernel_strcpy(char *, const char *);
void *kernel_memcpy(void *, const void *, size_t);
void *kernel_memmove(void *, const void *, size_t);

int kernel_atoi(const char *);
long int kernel_atol(const char *);

int kernel_access(const char *, int);
int kernel_open(const char *, int);
int kernel_close(int);

int kernel_putchar(int);
int kernel_puts(const char *);
int kernel_printf(const char *, ...);
int kernel_getchar(void);
//int kernel_gets(char *, size_t);
char *kernel_gets(char *, size_t);

int kernel_getx(void);

void *kernel_malloc(size_t);
void kernel_free(void *);

time_t kernel_get_bios_time(void);

char *convert_to_ascii(char *, int, ...);

int gateA20(int);
int get_code_end(void);
int get_memsize(int);
int get_mmap_entry(struct mmar_desc *, int);
int getrtsecs(void);

void init_memory(void);
void *get_kernel_heap(void);
void kernel_malloc_init(void *);

//void set_int20_handler(void);
//void unset_int20_handler(void);

void get_gdtr(struct td *);
//void set_gdtr(const struct td *);
void set_segment_tss(struct sd *, void *base);
void set_segment_descriptor(struct sd *, unsigned int, void *, int);

void get_idtr(struct td *);
void set_idtr(const struct td *);
void set_gate_handler(struct gd *, int (*)(), int, int);

void init_idt(void);

void init_pic(void);

int int20_handler();
int int21_handler(void);
int int27_handler(void);
int int80_handler(int, ...);

void init_timer(void);

int keycode_to_ascii(int);

//extern void *heap;

extern unsigned long int saved_mem_upper;
extern unsigned long int saved_mem_lower;
extern unsigned long int saved_mmap_addr;
extern unsigned long int saved_mmap_length;

#endif
