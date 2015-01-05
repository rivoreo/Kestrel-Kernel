/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "shared.h"
#include <kestrel/kernel.h>
#include <kestrel/graphics.h>
#include <kestrel/errno.h>
#include <kestrel/keymaps.h>

#define BUF_SIZE 64

#define IsCaps	((capslock_pressed && !shift_pressed) || (!capslock_pressed && shift_pressed))

/* Default SHIFT NOT PRESSED */
static int shift_pressed = 0;
static int shift_checked = 1;
static int capslock_pressed = 0;
static int capslock_checked = 1;

static int number_locked = 0;

int console_getkey(void);
void console_putchar(int);

int console_getxy(void);
void console_gotoxy(int, int);

struct char_buffer {
	volatile int front, rear;
	volatile unsigned char line;
	volatile char data[BUF_SIZE];
} input_buffer;

void input_buffer_put(int);
int input_buffer_get(void);

int kernel_printf(const char *format, ...) {
	int *dataptr = (int *)(void *)&format;
	char c, *ptr, str[16];
	char pad;
	int width;
	int r = 0;

	if(!format) {
		errno = EINVAL;
		return -1;
	}
	dataptr++;

	errno = 0;

	while ((c = *(format++))) {
		if(c == '%') {
			pad = ' ';
			width = 0;
			c = *(format++);

find_specifier:
			switch(c & 0xff) {
				case 'l': c = (*(format++) | (c << 8)); goto find_specifier;
				case 'd': case 'x': case 'X':  case 'u':
					*convert_to_ascii(str, c, *((unsigned long *)dataptr++)) = 0;
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
					if(!ptr) ptr = "(null)";
					while ((c = *(ptr++)) != 0) {
						kernel_putchar(c);
						r++;
					}
					break;
				case '0':
					pad = '0';
				case '1' ... '9':
					width = c - '0';
					while((c = *(format++)) >= '0' && c <= '9') {
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
	kernel_putchar('\n');
	return r;
}

int kernel_putchar(int c) {
	if(c == '\n') kernel_putchar('\r');
	else if(c == '	') {
		int x = 9 - kernel_getx() % 8;
		while(--x) kernel_putchar(' ');
		return c;
	}
#ifdef SUPPORT_GRAPHICS
	if(graphics_inited) return graphics_putchar(c);
#endif
	__asm__("cli\n");
	console_putchar(c);
	__asm__("sti\n");
	return c;
}

#if 0
int kernel_gets(char *buffer, size_t max_len) {
	int r = 0;
	while(max_len--) {
		int c = kernel_getchar();
		if(!c || c == -1 || c == '\r' || c == '\n') break;
		*(buffer++) = c;
		r++;
	}
	*buffer = 0;
	return r;
}
#else
char *kernel_gets(char *buffer, size_t max_len) {
	int count = 0;
	while(max_len--) {
		int c = kernel_getchar();
		//kernel_printf("gets debug: c = %d, count = %d\n", c, count);
		if(!c || c == -1 || c == '\r' || c == '\n') break;
		if(c == 8) {
			if(count) {
				kernel_printf("%c %c", c, c);
				count--;
			}
			continue;
		}
		if(c == '	') continue;
		buffer[count++] = c;
	}
	buffer[count] = 0;
	return buffer;
}
#endif

int kernel_getchar() {
#if 0
	int c = console_getkey();
#else
	while(input_buffer.front == input_buffer.rear || !input_buffer.line) __asm__("hlt\n");
	int c = input_buffer_get();
	if(c == -1) return -1;
	c &= 0xff;
	//if(c != 8 && c != '	') kernel_putchar(c);
	return c;
#endif
}

int kernel_getxy() {
	int xy =
#ifdef SUPPORT_GRAPHICS
		graphics_inited ? graphics_getxy() :
#endif
		console_getxy();
	return xy;
}

int kernel_getx() {
	return kernel_getxy() >> 8;
}

int kernel_gety() {
	return kernel_getxy() & 0xff;
}

void kernel_gotoxy(int x, int y) {
#ifdef SUPPORT_GRAPHICS
	graphics_inited ? graphics_gotoxy(x, y) :
#endif
	console_gotoxy(x, y);
}

int keycode_to_ascii(int code) {
	switch(code) {
		case 0x1:
			return 0x1b;
		case 0x2 ... 0xb:
			if(!shift_pressed) {
				if(code == 0xb) return 0x30;
				return code + 0x2f;
			}else{
				return KEYMAP5_UP[code - 0x2];
			}
		case 0xc:
			return (shift_pressed ? KEYMAP4_UP : KEYMAP4)[0];
		case 0xd:
			return (shift_pressed ? KEYMAP4_UP : KEYMAP4)[1];
		case 0xe:
			return 8;
		case 0xf ... 0x1c:
			return (IsCaps ? KEYMAP1_UP : KEYMAP1)[code - 0xf];
		case 0x1d:
			return 0x1;
		case 0x1e ... 0x29:
			return (IsCaps ? KEYMAP2_UP : KEYMAP2)[code - 0x1e];
		case 0x2b ... 0x35:
			return (IsCaps ? KEYMAP3_UP : KEYMAP3)[code - 0x2b];
		case 0x37:
			return '*';		// Extra
		case 0x39:
			return ' ';
		case 0x3a:
			if(!capslock_pressed){
				capslock_pressed = 1;
			}else{
				capslock_pressed = 0;
			}
			/* Reset capslock check status, to check capslock status again when next time. */
			capslock_checked = 0;
			return -2;
			/* if(capslock == 0x0010){
				kernel_printf("capslock pressed\n");
				return -2;
			} */
		case 0x45:
			number_locked = !number_locked;
			return -2;
		case 0x47 ... 0x53:
			if(number_locked) {
				return KEYMAP_EXTRA_NUMBER[code - 0x47];
			}
			switch(code) {
				case 0x4a:
					return '-';
				case 0x4e:
					return '+';
			}
			return -2;

		/* Press */
		case L_SHIFT_P:
		case R_SHIFT_P:
			shift_checked = 0;
			shift_pressed = 1;
			/* kernel_printf("shift pressed\n"); */
			return -2;

		/* Release */
		case L_SHIFT_R:
		case R_SHIFT_R:
			shift_checked = 0;
			shift_pressed = 0;
			/* kernel_printf("shift releaseed\n"); */
			return -2;

	}
	return -2;
}

void input_buffer_put(int code) {
	//kernel_printf("function: char_buffer_add(%d)\n", code);
/*
	__asm__("	movb	$0x61, %%al\n"
		"	outb	%%al, $0x20\n"
		::: "%eax");*/

	int c = keycode_to_ascii(code);
	if(!shift_checked) {
		shift_checked = 1;
		return;
	}
	if(!capslock_checked) {
		capslock_checked = 1;
		return;
	}
	switch(c) {
		case -2:
			return;
		case 8: {
			if(input_buffer.front == input_buffer.rear) return;
			//printf("%d, %d\n", input_buffer.front, input_buffer.rear);
			int xy = kernel_getxy();
			int x = xy >> 8, y;
			if(x) {
				x--;
				y = xy & 0xff;
			} else {
				x = 79;
				y = (xy & 0xff) - 1;
				kernel_gotoxy(x, y);	// XXX
				return;			// XXX
			}
			//kernel_putchar(8);
			kernel_gotoxy(x, y);
			kernel_putchar(' ');
			kernel_gotoxy(x, y);
			//kernel_putchar(8);
			input_buffer.front--;
			return;
		}
		case 1:
		case '	':
			break;
		case '\r':
			input_buffer.line = 1;
		default:
			kernel_putchar(c);
			break;
	}
	__asm__("cli\n");
	int reversed = input_buffer.front < input_buffer.rear;
	if(reversed && input_buffer.rear - input_buffer.front < 2) return;
	if(input_buffer.front + 1 == BUF_SIZE) {
		if(!input_buffer.rear) return;
		input_buffer.data[input_buffer.front] = c;
		input_buffer.front = 0;
		return;
	}
	input_buffer.data[input_buffer.front++] = c;
	//if(!reversed && input_buffer.front == BUF_SIZE) input_buffer.front = 0;
}

int input_buffer_get() {
	if(input_buffer.front == input_buffer.rear) return -1;
	int r = input_buffer.data[input_buffer.rear++];
	if(input_buffer.rear == BUF_SIZE) input_buffer.rear = 0;
	if(input_buffer.front == input_buffer.rear) input_buffer.line = 0;
	return r;
}
