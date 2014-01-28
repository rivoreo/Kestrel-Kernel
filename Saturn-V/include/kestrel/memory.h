/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _KESTREL_MEMORY_H
#define _KESTREL_MEMORY_H

#include <kestrel/types.h>

#define kernel_alloc kernel_malloc

struct mem_control_block {
	int is_available;
	int size;
}

void *kernel_malloc(size_t);
void kernel_free(void *);
void kernel_malloc_init();

#endif
