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

/*	Process the command line
	Seek out the command_table whose command name is command;

	return value: if found, returns a pointer to type command_t, or NULL if not found.
*/
command_t *find_command(char *command) {
	char *ptr;
	//char c;
	command_t **builtin;

	/* Find the first space and terminate the command name.  */
	ptr = command;
	while(*ptr != ' ' && *ptr != '\t') {
		if(!*ptr) {
			ptr[1] = 0;
			break;
		}
		ptr++;
	}

	//c = *ptr;
	*ptr = 0;

	/*   */
	for(builtin = command_table; *builtin != 0; builtin++) {
		int r = kernel_strcmp(command, (*builtin)->name);

		if(r == 0) {
			/* Found */
			//*ptr = c;		// keep the command without arguments
			return *builtin;
		} else if(r < 0) break;
	}

	/* Not found */
	//*ptr = c;		// keep the command without arguments
	return NULL;
}
