/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/types.h>

size_t kernel_strlen(const char *s) {
	size_t len = 0;
	while(*(s++)) len++;
	return len;
}

void *kernel_memcpy(void *to, const void *from, size_t n) {
#ifdef __i386__
       /* This assembly code is stolen from
	* linux-2.4.22/include/asm-i386/string.h
	* It assumes ds=es=data space, this should be normal.
	*/
	int d0, d1, d2;
	__asm__ __volatile__(
		"rep ; movsl\n\t"
		"testb $2,%b4\n\t"
		"je 1f\n\t"
		"movsw\n"
		"1:\ttestb $1,%b4\n\t"
		"je 2f\n\t"
		"movsb\n"
		"2:"
		: "=&c" (d0), "=&D" (d1), "=&S" (d2)
		:"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
		: "memory");
	return to;
#else
	char *d = (char *)to, *s = (char *)from;
	while(n--) *d++ = *s++;
	return to;
#endif
}
