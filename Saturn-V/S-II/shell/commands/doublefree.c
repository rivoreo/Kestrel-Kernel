/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/memory.h>

int doublefree_command(int argc, char **argv) {
	if(argc != 2) return 1;
	void *address = malloc(2);
	int times = kernel_atoi(argv[1]);
	for(;times > 0;times--) {
		kernel_free(address);
	}
	kernel_printf("Woo!! This is double free\n");
	return 0;
}

