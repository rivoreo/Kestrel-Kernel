/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/shell.h>

#define DECLARE_COMMAND(N,U) static command_t c_##N = { #N, N##_command, U }

extern int argstest_command(int, char **);
extern int date_command(int, char**);
extern int echo_command(int, char **);
extern int help_command(int, char **);
extern int doublefree_command(int, char **);
extern int malloctest_command(int, char **);
extern int mem_command(int, char **);
extern int panic_command(int, char **);
extern int reboot_command(int, char **);

DECLARE_COMMAND(argstest, "argstest [<args>]");
DECLARE_COMMAND(date, "date [<option>] ... [+<format>]");
DECLARE_COMMAND(doublefree, "doublefree <free times>");
DECLARE_COMMAND(echo, "echo [-ne] <args>");
DECLARE_COMMAND(help, "help");
DECLARE_COMMAND(malloctest, "malloctest <count> [<size>]");
DECLARE_COMMAND(mem, "mem read|write <address> [<...>]");
DECLARE_COMMAND(panic, "panic <message> [<status>]");
DECLARE_COMMAND(reboot, "reboot");

command_t *command_table[] = {
	&c_argstest,
	&c_date,
	&c_doublefree,
	&c_echo,
	&c_help,
	&c_malloctest,
	&c_mem,
	&c_panic,
	&c_reboot,
	NULL
};
