/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/errno.h>
#include <kestrel/kernel.h>

int mem_command(int argc, char **argv) {
	if(argc < 3) {
usage:
		kernel_puts("Usage:");
		kernel_printf("	%s read <address> [<len>(=1)]\n", argv[0]);
		kernel_printf("	%s write <address> <byte> [<byte> ...]\n", argv[0]);
		return -1;
	}

	if(kernel_strcmp(argv[1], "write") == 0) {
		unsigned char *p = (unsigned char *)kernel_atol(argv[2]);
		argv += 2;
		while(*argv) *p = (unsigned char)kernel_atol(*argv++);
	} else if(kernel_strcmp(argv[1], "read") == 0) {
		const unsigned char *p = (unsigned char *)kernel_atol(argv[2]);
		int i = 1;
		if(argv[3]) i = kernel_atoi(argv[3]);
		while(i--) kernel_printf("%u ", *p++);
		kernel_putchar('\n');
	} else goto usage;
	return 0;
}
