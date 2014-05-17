
#ifndef _KESTREL_GRAPHICS_H
#define _KESTREL_GRAPHICS_H

#include <kestrel/console.h>

/* magic constant */
#define VIDEOMEM 0xA0000

/* pubilc */
extern int foreground, background, border, graphics_inited;

void graphics_set_splash(char *);
int set_videomode(int);
int graphics_putchar(int);
int graphics_getxy(void);
void graphics_gotoxy(int, int);
void graphics_cls(void);
void graphics_setcolorstate(color_state);
void graphics_setcolor(int, int, int, int);
int graphics_setcursor(int);
int graphics_init(void);
void graphics_end(void);

//int hex(int);
void graphics_set_palette(int, int, int, int);


/* function prototypes */
char *graphics_get_splash(void);
//static void graphics_cursor(int);

/* function prototypes for asm functions */
void *graphics_get_font();
void graphics_set_palette(int idx, int red, int green, int blue);
void set_int1c_handler();
void unset_int1c_handler();

extern short cursorX, cursorY;
extern char cursorBuf[16];

#endif
