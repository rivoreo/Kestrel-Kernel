/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/shell.h>
#include <mstring.h>

#define USE_ARRAY

void enter_shell() {
	//char *buffer = (char *)heap;
	//char *buffer = kernel_alloc(MAX_COMMAND * sizeof(char));
	char buffer[MAX_COMMAND];
	command_t *command;

	kernel_printf("Kestrel Kernel build-in shell\n\n");

	while(1) {
		char *commandline = buffer;
		kernel_printf("kestrel> ");
		kernel_gets(commandline, MAX_COMMAND - 1);
		kernel_putchar('\n');
		while(*commandline == ' ') commandline++;
		if(!*commandline) continue;
		command = find_command(commandline);
		if(!command) {
			kernel_printf("%s: command not found\n", commandline);
			last_status = 127;
			continue;
		}
		int _argc;
#ifndef USE_ARRAY
		char **_argv;
#endif
		int i = mstrcount(commandline);
		_argc = i;
#ifndef USE_ARRAY
		_argv = (char **)kernel_alloc((i + 1) * sizeof(char *));
#else
		char *_argv[i+1];
#endif
		_argv[i] = NULL;
		for(; i; i--) _argv[i-1] = mstrindex(commandline, i);
		last_status = command->exec(_argc, _argv);
#ifndef USE_ARRAY
		kernel_free(_argv);
#endif
	}
}
