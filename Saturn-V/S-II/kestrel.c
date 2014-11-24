/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/time.h>
#include <kestrel/graphics.h>
#include <kestrel/shell.h>
#include <kestrel/sched.h>
#include <kestrel/syscall.h>

void cmain() {
	kernel_puts("Kestrel " VERSION);
	init_idt();
	init_pic();
	//__asm__("hlt\n");
	init_timer();
	init_sched();
	init_memory();
	init_systemcalls();
	//__asm__("sti\n");
	//set_int13_handler();
#ifdef SUPPORT_GRAPHICS
	//if(!graphics_init()) kernel_perror("graphics_init");
#endif
	save_kernel_startup_clock();
	if(use_config_file && kernel_access(config_file, R_OK) == 0) {
		kernel_printf("Loading config file: %s ...\n", config_file);
		run_script(config_file);
	} else {
		kernel_putchar('\n');
		enter_shell();
		//__asm__("int	$0x21\n");
		while(1) {
			__asm__("sti\nhlt\n");
			//kernel_putchar('.');
		}
	}
}
