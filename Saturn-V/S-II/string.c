/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/types.h>
//#include <ctype.h>

size_t kernel_strlen(const char *s) {
	size_t len = 0;
	while(*(s++)) len++;
	return len;
}

int kernel_strcmp(const char *s1, const char *s2) {
	while(*s1 || *s2) {
		if(*s1 < *s2) return -1;
		else if(*s1 > *s2) return 1;
		s1++;
		s2++;
	}
	return 0;
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

long int kernel_atol(const char *s) {
	int c;			/* current char */
	long int total;		/* current total */
	int sign;		/* if '-', then negative, otherwise positive */

	while(isspace((unsigned char)*s)) s++;
	c = *s++;
	sign = c;		/* save sign indication */
	if (c == '-' || c == '+') c = *s++;
	total = 0;
	while (c >= '0' && c <= '9') {
		total = 10 * total + (c - '0');
		c = *s++;
	}
	if (sign == '-') return -total;
	return total;
}

int kernel_atoi(const char *s) {
	return (int)kernel_atol(s);
}

char *convert_to_ascii(char *buf, int c, ...) {
	unsigned long num = *((&c) + 1), mult = 10;
	char *ptr = buf;

	if (c == 'x' || c == 'X') mult = 16;

	if ((num & 0x80000000uL) && c == 'd') {
		num = (~num) + 1;
		*(ptr++) = '-';
		buf++;
	}

	do {
		int dig = num % mult;
		*(ptr++) = ((dig > 9) ? dig + c - 33 : '0' + dig);
	} while (num /= mult);

	/* reorder to correct direction!! */
	char *ptr1 = ptr - 1;
	char *ptr2 = buf;
	while (ptr1 > ptr2) {
		int tmp = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = tmp;
		ptr1--;
		ptr2++;
	}

	return ptr;
}
