/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/shell.h>

int use_config_file = 1;
int last_status = 0;

static void commandline_to_multistring(char *s) {
	char *space = " \t\r\n";
	//char *quote = "\"\'";
	char f = 1;
	while(1) {
		char *q;
		if(f) for(q = space; *q; q++) if(*s == *q) {
			*s = 0;
			break;
		}
		if(*s == '\"') {
			f ^= 1;
			kernel_memmove(s, s + 1, kernel_strlen(s) + 1);
			continue;
		}
		if(!*++s) break;
	}
	s[1] = 0;
}

/*	Process the command line

	Seek out the command_table whose command name is command;
	and convert the command line to multi string (ex.: "echo\0-n\0Hello\0World\0\0").

	return value: if found, returns a pointer to type command_t, or NULL if not found.
*/
command_t *find_command(char *command) {
	char *ptr;
	command_t **builtin;

	/* Find the first space and terminate the command name.  */
	ptr = command;
	while(1) {
		if(!*ptr) {
			ptr[1] = ptr[0] = 0;
			break;
		}
		if(*ptr == ' ' || *ptr == '\t') {
			*ptr++ = 0;
			commandline_to_multistring(ptr);
			break;
		}
		ptr++;
	}

	for(builtin = command_table; *builtin; builtin++) {
		if(kernel_strcmp(command, (*builtin)->name) == 0) return *builtin;	// Found
	}

	// Not found
	return NULL;
}
