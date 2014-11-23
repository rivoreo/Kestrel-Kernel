#include <kestrel/graphics.h>
#include <kestrel/kernel.h>

int graphics_command(int argc, char **argv) {
	if(argc < 2) {
usage:
		kernel_puts("Usage:");
		kernel_printf("	%s on\n", argv[0]);
		kernel_printf("	%s off\n", argv[0]);
		return -1;
	}

	if(kernel_strcmp(argv[1], "off") == 0) {
		if(!graphics_inited) {
			kernel_puts("Not in graphics mode");
			return 1;
		}
		graphics_end();
		return 0;
	} else if(kernel_strcmp(argv[1], "on") == 0) {
		if(graphics_inited) {
			kernel_puts("Graphics mode is already inited");
			return 1;
		}
		graphics_init();
		return 0;
	} else goto usage;
}