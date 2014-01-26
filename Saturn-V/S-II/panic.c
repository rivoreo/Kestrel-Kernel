/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/errno.h>
#include <kestrel/kernel.h>

void kernel_panic(const char *msg, int status) {
	kernel_printf("\nkernel panic: %s", msg);
	if(status) kernel_printf(" (%d)", status);
	kernel_putchar('\n');
	stop();
	while(1);
}

void panic(const char *msg) {
	kernel_panic(msg, errno);
}
