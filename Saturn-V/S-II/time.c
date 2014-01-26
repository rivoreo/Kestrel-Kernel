/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 */

#include <kestrel/kernel.h>
#include <kestrel/time.h>
#include <kestrel/asm/io.h>

int gettime(struct tm *time) {
	//return get_bios_time();
	do 
	{
		time->tm_sec = CMOS_READ (0);
		time->tm_min = CMOS_READ (2);
		time->tm_hour = CMOS_READ (4);
		time->tm_mday = CMOS_READ (7);
		time->tm_mon = CMOS_READ (8);
		time->tm_year = CMOS_READ (9);
	}
	while (time->tm_sec != CMOS_READ (0));
	BCD_TO_BIN (time->tm_sec);
	BCD_TO_BIN (time->tm_min);
	BCD_TO_BIN (time->tm_hour);
	BCD_TO_BIN (time->tm_mday);
	BCD_TO_BIN (time->tm_mon);
	BCD_TO_BIN (time->tm_year);
	//time->tm_mon--;
	return 0;
}

/*
 * This isn't the library routine, it is only used in the kernel.
 * as such, we don't care about years<1970 etc, but assume everything
 * is ok. Similarly, TZ etc is happily ignored. We just do everything
 * as easily as possible. Let's find something public for the library
 * routines (although I think minix times is public).
 */
/*
 * PS. I hate whoever though up the year 1970 - couldn't they have gotten
 * a leap-year instead? I also hate Gregorius, pope or no. I'm grumpy.
 */
#define MINUTE 60		// 1 分钟的秒数。
#define HOUR (60*MINUTE)	// 1 小时的秒数。
#define DAY (24*HOUR)		// 1 天的秒数。
#define YEAR (365*DAY)		// 1 年的秒数。

/* interestingly, we assume leap-years */
/* 有趣的是我们考虑进了闰年 */
// 下面以年为界限，定义了每个月开始时的秒数时间数组。
static int month[12] = {
	0,
	DAY * (31),
	DAY * (31 + 29),
	DAY * (31 + 29 + 31),
	DAY * (31 + 29 + 31 + 30),
	DAY * (31 + 29 + 31 + 30 + 31),
	DAY * (31 + 29 + 31 + 30 + 31 + 30),
	DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
	DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
	DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
	DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
	DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};

// 该函数计算从1970年1月1日0时（Unix时间）起到开机当日经过的秒数，作为开机时间。
time_t mktime_sec(struct tm *tm)
{
	long res;
	int year;

	year = tm->tm_year - 70;	// 从70 年到现在经过的年数(2 位表示方式)，
	// 因此会有2000 年问题。
	/* magic offsets (y+1) needed to get leapyears right. */
	/* 为了获得正确的闰年数，这里需要这样一个魔幻偏值(y+1) */
	res = YEAR * year + DAY * ((year + 1) / 4);	// 这些年经过的秒数时间 + 每个闰年时多1 天
	res += month[tm->tm_mon];	// 的秒数时间，在加上当年到当月时的秒数。
	/* and (y+2) here. If it wasn't a leap-year, we have to adjust */
	/* 以及(y+2)。如果(y+2)不是闰年，那么我们就必须进行调整(减去一天的秒数时间)。 */
	if (tm->tm_mon > 1 && ((year + 2) % 4)) res -= DAY;
	res += DAY * (tm->tm_mday - 1);	// 再加上本月过去的天数的秒数时间。
	res += HOUR * tm->tm_hour;	// 再加上当天过去的小时数的秒数时间。
	res += MINUTE * tm->tm_min;	// 再加上1 小时内过去的分钟数的秒数时间。
	res += tm->tm_sec;		// 再加上1 分钟内已过的秒数。
	return res;			// 即等于从1970 年以来经过的秒数时间。
}

