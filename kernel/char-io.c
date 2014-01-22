/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "shared.h"

#define kernel_putchar console_putchar

int printf(const char *format, ...) {
	int *dataptr = (int *)(void *) &format;
	char c, *ptr, str[16];
	char pad;
	int width;
	int r = 0;

	if(!format) return -1;
	dataptr++;

	while ((c = *(format++))) {
		if(c == '%') {
			pad = ' ';
			width = 0;
			c = *(format++);

find_specifier:
			switch (c) {
				case 'd': case 'x':	case 'X':  case 'u':
					//*convert_to_ascii(str, c, *((unsigned long *)dataptr++)) = 0;
					width -= kernel_strlen(str);
					if (width > 0) while(width--) {
						kernel_putchar(pad);
						r++;
					}
					ptr = str;
					while (*ptr) {
						kernel_putchar(*(ptr++));
						r++;
					}
					break;

				case 'c':
					if (width > 0) while(--width) {
						kernel_putchar(pad);
						r++;
					}
					kernel_putchar((*(dataptr++)) & 0xff);
					r++;
					break;

				case 's':
					width -= kernel_strlen((char *)*(dataptr));
					if (width > 0) while(width--) {
						kernel_putchar(pad);
						r++;
					}
					ptr = (char *)*(dataptr++);
					while ((c = *(ptr++)) != 0) {
						kernel_putchar (c);
						r++;
					}
					break;
				case '0':
					pad = '0';
				case '1' ... '9':
					width = c - '0';
					while ((c = *(format++)) >= '0' && c <= '9') {
						width = width * 10 + c - '0';
					}
					goto find_specifier;
			}
		} else {
			kernel_putchar(c);
			r++;
		}
	}

	return r;
}

int kernel_puts(const char *s) {
	int r = 1;
	while(*s) {
		if(kernel_putchar(*(s++)) < 0) return -1;
		r++;
	}
	putchar('\r');
	putchar('\n');
	return r;
}
