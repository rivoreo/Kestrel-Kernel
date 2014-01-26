/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _KESTREL_SHELL_H
#define _KESTREL_SHELL_H

#define COMMANDLINE_BUFFER 0x2000
#define MAX_COMMAND 1600

#ifndef ASM_FILE
typedef struct {
	char *name;
	int (*exec)(int, char **);
	char *usage;
} command_t;

void enter_shell(void) __attribute__((__noreturn__));
void run_script(const char *) __attribute__((__noreturn__));
command_t *find_command(char *);

extern char config_file[];
extern int use_config_file;
extern int last_status;

extern command_t *command_table[];
#endif

#endif
