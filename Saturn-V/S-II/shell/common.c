#include <kestrel/kernel.h>
#include <kestrel/shell.h>

int use_config_file = 1;

command_t *find_command(char *command) {
	char *ptr;
	char c;
	command_t **builtin;

	/* Find the first space and terminate the command name.  */
	ptr = command;
	while(*ptr && *ptr != ' ' && *ptr != '\t') ptr++;

	c = *ptr;
	*ptr = 0;

	/* Seek out the builtin whose command name is COMMAND.  */
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
