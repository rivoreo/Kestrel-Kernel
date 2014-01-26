#ifndef _TIME_H
#define _TIME_H

typedef long time_t;		// 从GMT 1970 年1 月1 日开始的以秒计数的时间（日历时间）。
#endif

#define CLOCKS_PER_SEC 100	// 系统时钟滴答频率，100HZ。
#define CMOS_READ(addr) ({outb_p(0x80|addr,0x70); inb_p(0x71);})
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)	// 将BCD 码转换成数字。

typedef long clock_t;		// 从进程开始系统经过的时钟滴答数。

struct tm
{
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

int gettime(struct tm *);

