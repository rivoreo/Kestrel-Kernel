/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>

int malloctest_command(int argc, char **argv) {
	if(argc < 2) {
		kernel_printf("Usage: %s <count> [<size of allocates>]\n", argv[0]);
		return 1;
	}
	unsigned int count = atoi(argv[1]);
	size_t alloc_size = 8;
	if(!count) {
		puts("\"count = 0\" is invalid");
		return 1;
	}
	if(argc > 2) alloc_size = atoi(argv[2]);
	void **l = malloc(count * sizeof(void *));
	printf("l = 0x%lx\n", (long int)l);
	int i;
	for(i=0; i<count; i++) {
		l[i] = malloc(alloc_size);
		printf("l[%u] = 0x%lx\n", i, l[i]);
	}
	puts("Now, free thems");
	for(i=0; i<count; i++) free(l[i]);
	free(l);
	return 0;
}
