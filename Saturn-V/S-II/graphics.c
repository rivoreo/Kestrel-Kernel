/* graphics.c - graphics mode support for GRUB */
/* Implemented as a terminal type by Jeremy Katz <katzj@redhat.com> based
 * on a patch by Paulo César Pereira de Andrade <pcpa@conectiva.com.br>
 */


#ifdef SUPPORT_GRAPHICS

#include "shared.h"
//#include <kestrel/graphics.h>
//#include <kestrel/kernel.h>
//#include <kestrel/console.h>

static int saved_videomode = 0;
static unsigned char *font8x16 = 0;

int outline = 0;
int disable_space_highlight = 0;
int graphics_inited = 0;
char splashimage[64];

#define VSHADOW VSHADOW1
/* 8x16 dot array, total chars = 80*30. plano size = 80*30*16 = 38400 bytes */
static unsigned char *VSHADOW1 = (unsigned char *)0x3A0000;	//unsigned char VSHADOW1[38400];
static unsigned char *VSHADOW2 = (unsigned char *)0x3A9600;	//unsigned char VSHADOW2[38400];
static unsigned char *VSHADOW4 = (unsigned char *)0x3B2C00;	//unsigned char VSHADOW4[38400];
static unsigned char *VSHADOW8 = (unsigned char *)0x3BC200;	//unsigned char VSHADOW8[38400];
/* text buffer has to be kept around so that we can write things as we
 * scroll and the like */
//static unsigned short text[80 * 30];
static unsigned long *text = (unsigned long *)0x3C5800; // length in bytes = 80*30*4

/* constants to define the viewable area */
const int x0 = 0;
const int x1 = 80;
const int y0 = 0;
const int y1 = 30;

/* why do these have to be kept here? */
int foreground = (63 << 16) | (63 << 8) | (63), background = 0, border = 0;

/* current position */
static int fontx = 0;
static int fonty = 0;

/* global state so that we don't try to recursively scroll or cursor */
static int no_scroll = 0;

/* color state */
static int graphics_standard_color = A_NORMAL;
static int graphics_normal_color = A_NORMAL;
static int graphics_highlight_color = A_REVERSE;
static int graphics_helptext_color = A_NORMAL;
static int graphics_heading_color = A_NORMAL;
static int graphics_current_color = A_NORMAL;
static color_state graphics_color_state = COLOR_STATE_STANDARD;


/* graphics local functions */
static void graphics_cursor(int);
static void graphics_setxy(int, int);
static void graphics_scroll(void);
//static int read_image(char *s);

/* FIXME: where do these really belong? */
static inline void outb(unsigned short port, unsigned char val)
{
	__asm __volatile ("outb %0,%1"::"a" (val), "d" (port));
}

static void MapMask(int value) {
	outb(0x3c4, 2);
	outb(0x3c5, value);
}

/* bit mask register */
static void BitMask(int value) {
	outb(0x3ce, 8);
	outb(0x3cf, value);
}



///* Set the splash image */
//void graphics_set_splash(char *splashfile) {
//    grub_strcpy(splashimage, splashfile);
//}

///* Get the current splash image */
//char *graphics_get_splash(void) {
//    return splashimage;
//}

/* Initialize a vga16 graphics display with the palette based off of
 * the image in splashimage.  If the image doesn't exist, leave graphics
 * mode.  */
int graphics_init(void)
{
	/* graphics mode will corrupt the extended memory. so we should
	 * invalidate the kernel_type. */
	if (!graphics_inited) {
		saved_videomode = set_videomode(0x12);
	}

	font8x16 = (unsigned char *) graphics_get_font(); /* code in asm.S */

	graphics_inited = 1;
	errno = 0;

	/* make sure that the highlight color is set correctly */
	graphics_highlight_color = ((graphics_normal_color >> 4) | 
			((graphics_normal_color & 0xf) << 4));

	errno = 0;
	return 1;
}

/* Leave graphics mode */
void graphics_end(void)
{
	if (graphics_inited) {
		set_videomode (saved_videomode);
		graphics_inited = 0;
	}
	errno = 0;
}

/* Print ch on the screen.  Handle any needed scrolling or the like */
int graphics_putchar(int ch)
{
	ch &= 0xff;

	//graphics_cursor(0);

	if (ch == '\n') {
		if (fonty + 1 < y1) {
			graphics_gotoxy(fontx, fonty + 1);
		} else {
			graphics_cursor(0);
			graphics_scroll();
			graphics_cursor(1);
		}
		//graphics_cursor(1);
		return ch;
	} else if (ch == '\r') {
		graphics_gotoxy(x0, fonty);
		//graphics_cursor(1);
		return ch;
	}

	//graphics_cursor(0);

	text[fonty * 80 + fontx] = ch;
	text[fonty * 80 + fontx] &= 0x00ff;
	if (graphics_current_color & 0xf0) {
		text[fonty * 80 + fontx] |= 0x10000;//0x100;
	}

	graphics_cursor(0);

	if ((fontx + 1) >= x1) {
		if (fonty + 1 < y1) {
			graphics_setxy(x0, fonty + 1);
		} else {
			graphics_setxy(x0, fonty);
			graphics_scroll();
		}
	} else {
		graphics_setxy(fontx + 1, fonty);
	}

	graphics_cursor(1);
	errno = 0;
	return ch;
}

/* get the current location of the cursor */
int graphics_getxy(void)
{
	return (fontx << 8) | fonty;
}

void graphics_gotoxy(int x, int y)
{
	graphics_cursor(0);
	graphics_setxy(x, y);
	graphics_cursor(1);
}

void graphics_cls(void)
{
	int i;
	unsigned char *mem, *s1, *s2, *s4, *s8;

	graphics_cursor(0);
	graphics_gotoxy(x0, y0);

	mem = (unsigned char*)VIDEOMEM;
	s1 = (unsigned char*)VSHADOW1;
	s2 = (unsigned char*)VSHADOW2;
	s4 = (unsigned char*)VSHADOW4;
	s8 = (unsigned char*)VSHADOW8;

	for (i = 0; i < 80 * 30; i++)
		text[i] = ' ';
	graphics_cursor(1);

	BitMask(0xff);

	/* plano 1 */
	MapMask(1);
	kernel_memcpy(mem, s1, 38400);

	/* plano 2 */
	MapMask(2);
	kernel_memcpy(mem, s2, 38400);

	/* plano 3 */
	MapMask(4);
	kernel_memcpy(mem, s4, 38400);

	/* plano 4 */
	MapMask(8);
	kernel_memcpy(mem, s8, 38400);

	MapMask(15);

	errno = 0;
}

void graphics_setcolorstate(color_state state)
{
	switch (state) {
		case COLOR_STATE_STANDARD:
			graphics_current_color = graphics_standard_color;
			break;
		case COLOR_STATE_NORMAL:
			graphics_current_color = graphics_normal_color;
			break;
		case COLOR_STATE_HIGHLIGHT:
			graphics_current_color = graphics_highlight_color;
			break;
		case COLOR_STATE_HELPTEXT:
			graphics_current_color = graphics_helptext_color;
			break;
		case COLOR_STATE_HEADING:
			graphics_current_color = graphics_heading_color;
			break;
		default:
			graphics_current_color = graphics_standard_color;
			break;
	}

	graphics_color_state = state;
}

void graphics_setcolor(int normal_color, int highlight_color, int helptext_color, int heading_color)
{
	graphics_normal_color = normal_color;
	graphics_highlight_color = highlight_color;
	graphics_helptext_color = helptext_color;
	graphics_heading_color = heading_color;

	graphics_setcolorstate (graphics_color_state);
}

int graphics_setcursor(int on)
{
	/* FIXME: we don't have a cursor in graphics */
	errno = ENOSYS;
	return 0;
}

/*
static int read_image(char *s)
{
	return 0;
}
*/

/* Convert a character which is a hex digit to the appropriate integer */
/*
static int hex(int v)
{
	if (v >= 'A' && v <= 'F')
		return (v - 'A' + 10);
	if (v >= 'a' && v <= 'f')
		return (v - 'a' + 10);
	return (v - '0');
}
*/

/* move the graphics cursor location to col, row */
static void graphics_setxy(int col, int row)
{
	if (col >= x0 && col < x1) {
		fontx = col;
		cursorX = col << 3;
	}

	if (row >= y0 && row < y1) {
		fonty = row;
		cursorY = row << 4;
	}
}

/* scroll the screen */
static void graphics_scroll(void)
{
	int i, j;

	/* we don't want to scroll recursively... that would be bad */
	if (no_scroll) return;
	no_scroll = 1;

	/* move everything up a line */
	for (j = y0 + 1; j < y1; j++) {
		graphics_gotoxy (x0, j - 1);

		for (i = x0; i < x1; i++) {
			graphics_putchar (text[j * 80 + i]);
		}
	}

	/* last line should be blank */
	graphics_gotoxy (x0, y1 - 1);

	for (i = x0; i < x1; i++) graphics_putchar (' ');

	graphics_setxy (x0, y1 - 1);

	no_scroll = 0;
}


static unsigned char chr[16 << 2];
static unsigned char mask[16];

static void graphics_cursor(int set)
{
	unsigned char *pat, *mem, *ptr;
	int i, ch, offset;
	int invert = 0;

	if (set && no_scroll) return;

	offset = cursorY * 80 + fontx;
	ch = text[fonty * 80 + fontx] & 0xff;
	if (ch != ' ' || ! disable_space_highlight) {
		invert = (text[fonty * 80 + fontx] & /*0xff00*/ 0xffff0000) != 0;
	}
	pat = font8x16 + (ch << 4);

	mem = (unsigned char*)VIDEOMEM + offset;

	if (set) {
		MapMask(15);
		ptr = mem;
		for (i = 0; i < 16; i++, ptr += 80) {
			cursorBuf[i] = pat[i];
			*ptr = ~pat[i];
		}
		return;
	}

	if (outline) for (i = 0; i < 16; i++) {
		mask[i] = pat[i];
		if (i < 15) mask[i] |= pat[i+1];
		if (i > 0) mask[i] |= pat[i-1];
		mask[i] |= (mask[i] << 1) | (mask[i] >> 1);
		mask[i] = ~(mask[i]);
	}

	for (i = 0; i < 16; i++, offset += 80) {
		unsigned char m, p, c1, c2, c4, c8;

		p = pat[i];

		if (invert) {
			p = ~p;
			chr[i     ] = p;
			chr[16 + i] = p;
			chr[32 + i] = p;
			chr[48 + i] = p;
			continue;
		}

		c1 = ((unsigned char*)VSHADOW1)[offset];
		c2 = ((unsigned char*)VSHADOW2)[offset];
		c4 = ((unsigned char*)VSHADOW4)[offset];
		c8 = ((unsigned char*)VSHADOW8)[offset];

		if (outline) {
			m = mask[i];

			c1 &= m;
			c2 &= m;
			c4 &= m;
			c8 &= m;
		}

		c1 |= p;
		c2 |= p;
		c4 |= p;
		c8 |= p;

#if 0	
		if (invert) {
			c1 = ~c1;
			c2 = ~c2;
			c4 = ~c4;
			c8 = ~c8;
		}
#endif

		chr[i     ] = c1;
		chr[16 + i] = c2;
		chr[32 + i] = c4;
		chr[48 + i] = c8;
	}

	offset = 0;
	for (i = 1; i < 16; i <<= 1, offset += 16) {
		int j;

		MapMask(i);
		ptr = mem;
		for (j = 0; j < 16; j++, ptr += 80) {
			*ptr = chr[j + offset];
		}
	}

	MapMask(15);
}

#endif /* SUPPORT_GRAPHICS */
