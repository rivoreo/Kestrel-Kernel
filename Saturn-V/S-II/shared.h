#ifndef _KESTREL_KERNEL_SHARED_H
#define _KESTREL_KERNEL_SHARED_H

#define EXT_C(sym) sym
#define RAW_ADDR(x) (x)
#define RAW_SEG(x) (x)

#include <kestrel/shell.h>

#define MAXINT 0xffffffff

/* The size of the drive map.  */
#define DRIVE_MAP_SIZE		8

/* The size of the drive_map_slot struct.  */
#define DRIVE_MAP_SLOT_SIZE	24

/* The size of the key map.  */
#define KEY_MAP_SIZE		128

#define HMA_ADDR		0x2B0000

#define CR0_PE_ON	0x1
#define CR0_PE_OFF	0xfffffffe
#define PROT_MODE_CSEG	0x8
#define PROT_MODE_DSEG  0x10
#define PSEUDO_RM_CSEG	0x18
#define PSEUDO_RM_DSEG	0x20
#define STACKOFF	COMMANDLINE_BUFFER		/* (0x2000 - 0x10) */
#define PROTSTACKINIT   (FSYS_BUF - 0x10)

#define ENTRY(x) .globl EXT_C(x) ; EXT_C(x):
#define VARIABLE(x) ENTRY(x)

/* Codes for getchar. */
#define ASCII_CHAR(x)   ((x) & 0xFF)
#define KEY_LEFT        0x4B00
#define KEY_RIGHT       0x4D00
#define KEY_UP          0x4800
#define KEY_DOWN        0x5000
#define KEY_IC          0x5200	/* insert char */
#define KEY_DC          0x5300	/* delete char */
#define KEY_BACKSPACE   0x0E08
#define KEY_HOME        0x4700
#define KEY_END         0x4F00
#define KEY_NPAGE       0x5100
#define KEY_PPAGE       0x4900
#define A_NORMAL        0x7
#define A_REVERSE       0x70

#define memcpy kernel_memcpy
#define memncpy kernel_memncpy
#define strncat kernel_strncat
#define strstr kernel_strstr
#define memcmp kernel_memcmp
#define strcmp kernel_strcmp
#define tolower kernel_tolower
#define strlen kernel_strlen
#define strcat kernel_strcat
#define strcpy kernel_strcpy
#define strncpy kernel_strncpy
#define sprintf kernel_sprintf

#undef putchar
#define putchar kernel_putchar
#define puts kernel_puts
#define printf kernel_printf


/* see typedef gfx_data_t below */
#define gfx_ofs_v1_ok			0x00
#define gfx_ofs_v1_mem_start		0x04
#define gfx_ofs_v1_mem_cur		0x08
#define gfx_ofs_v1_mem_max		0x0c
#define gfx_ofs_v1_code_seg		0x10
#define gfx_ofs_v1_jmp_table		0x14
#define gfx_ofs_v1_sys_cfg		0x44
#define gfx_ofs_v1_cmdline		0x64
#define gfx_ofs_v1_cmdline_len		0x68
#define gfx_ofs_v1_menu_list		0x6c
#define gfx_ofs_v1_menu_default_entry	0x70
#define gfx_ofs_v1_menu_entries		0x74
#define gfx_ofs_v1_menu_entry_len	0x78
#define gfx_ofs_v1_args_list		0x7c
#define gfx_ofs_v1_args_entry_len	0x80
#define gfx_ofs_v1_timeout		0x84
#define gfx_ofs_v1_mem_file		0x88
#define gfx_ofs_v1_mem_align		0x8c

#define gfx_ofs_v2_ok			0x00
#define gfx_ofs_v2_code_seg		0x04
#define gfx_ofs_v2_jmp_table		0x08
#define gfx_ofs_v2_sys_cfg		0x38
#define gfx_ofs_v2_cmdline		0x6c
#define gfx_ofs_v2_cmdline_len		0x70
#define gfx_ofs_v2_menu_list		0x74
#define gfx_ofs_v2_menu_default_entry	0x78
#define gfx_ofs_v2_menu_entries		0x7c
#define gfx_ofs_v2_menu_entry_len	0x80
#define gfx_ofs_v2_args_list		0x84
#define gfx_ofs_v2_args_entry_len	0x88
#define gfx_ofs_v2_timeout		0x8c

#ifndef ASM_FILE
#include <kestrel/console.h>
#include <kestrel/errno.h>
#include <kestrel/graphics.h>
#include <kestrel/kernel.h>
#include <kestrel/types.h>
#endif

#endif
