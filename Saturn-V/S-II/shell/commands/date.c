/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/ 

#include <kestrel/kernel.h>
#include <kestrel/time.h>

int date_command(int argc, char **argv) {
	struct tm cdate;
		gettime(&cdate);
		kernel_printf(	"The current UTC time is:\n"
				"%d/%d/%d\n%d:%d:%d\n",
				cdate.tm_year, cdate.tm_mon,
				cdate.tm_mday, cdate.tm_hour,
				cdate.tm_min, cdate.tm_sec);
	return 0;
}

