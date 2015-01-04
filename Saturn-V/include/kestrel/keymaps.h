#ifndef _KEYMAPS_H
#define _KEYMAPS_H

#define dovrak

/*************************************
* en_US Keyboard
**************************************/
#ifdef en_US

#define L_SHIFT_P	0x2a
#define R_SHIFT_P	0x36
#define L_SHIFT_R	0xAA
#define R_SHIFT_R	0xB6

static char en_keymap_num[] = "!@#$%^&*()_+";
static char en_keymap1[] = "	qwertyuiop[]\r";
static char en_keymap2[] = "asdfghjkl;'`";
static char en_keymap3[] = "\\zxcvbnm,./";

static char extra_number_keymap[] = "789-456+1230.";

static char en_shift_keymap1[] = "	QWERTYUIOP{}\r";
static char en_shift_keymap2[] = "ASDFGHJKL:\"~";
static char en_shift_keymap3[] = "|ZXCVBNM<>?";

#endif

/*************************************
* dovrak Keyboard
**************************************/
#ifdef dovrak

#define L_SHIFT_P	0x2a
#define R_SHIFT_P	0x36
#define L_SHIFT_R	0xAA
#define R_SHIFT_R	0xB6

static char en_keymap_num[] = "!@#$%^&*()_+";
static char en_keymap1[] = "	',.pyfgcrl/=\r";
static char en_keymap2[] = "aoeuidhtns-`";
static char en_keymap3[] = "\\;qjkxbmwvz";

static char extra_number_keymap[] = "789-456+1230.";

static char en_shift_keymap1[] = "	\"<>PYFGCRL?+\r";
static char en_shift_keymap2[] = "AOEUIDHTNS_~";
static char en_shift_keymap3[] = "|:QJKXBMWVZ";

#endif

#endif
