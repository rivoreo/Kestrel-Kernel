/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _KESTREL_TIME_H
#define _KESTREL_TIME_H

#include <kestrel/types.h>

#define CLOCKS_PER_SEC 100	// 系统时钟滴答频率，100HZ。
#define CMOS_READ(addr) ({outb_p(0x80|addr,0x70); inb_p(0x71);})
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)	// 将BCD 码转换成数字。

struct tm {
	int tm_sec;			// 秒数 [0，59]。
	int tm_min;			// 分钟数 [ 0，59]。
	int tm_hour;			// 小时数 [0，59]。
	int tm_mday;			// 1 个月的天数 [0，31]。
	int tm_mon;			// 1 年中月份 [0，11]。
	int tm_year;			// 从1900 年开始的年数。
	int tm_wday;			// 1 星期中的某天 [0，6]（星期天 =0）。
	int tm_yday;			// 1 年中的某天 [0，365]。
	int tm_isdst;			// 夏令时标志。
};

// System Call
time_t time(time_t *);

int gettime(struct tm *);
time_t mktime_sec(struct tm *);
time_t get_startup_time(void);
void save_kernel_startup_time(void);

unsigned long int get_uptime(void);

#endif
