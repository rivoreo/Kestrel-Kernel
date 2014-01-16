/*
* linux/fs/super.c
*
* (C) 1991 Linus Torvalds
*/

/*
* super.c contains code to handle the super-block tables.
*/
#include <linux/config.h>	// 内核配置头文件。定义键盘语言和硬盘类型（HD_TYPE）可选项。
#include <linux/sched.h>	// 调度程序头文件，定义了任务结构task_struct、初始任务0 的数据，
// 还有一些有关描述符参数设置和获取的嵌入式汇编函数宏语句。
#include <linux/kernel.h>	// 内核头文件。含有一些内核常用函数的原形定义。
#include <asm/system.h>		// 系统头文件。定义了设置或修改描述符/中断门等的嵌入式汇编宏。

#include <errno.h>		// 错误号头文件。包含系统中各种出错号。(Linus 从minix 中引进的)。
#include <sys/stat.h>		// 文件状态头文件。含有文件或文件系统状态结构stat{}和常量。

int sync_dev (int dev);		// 对指定设备执行高速缓冲与设备上数据的同步操作。(fs/buffer.c,59)
void wait_for_keypress (void);	// 等待击键。(kernel/chr_drv/tty_io.c, 140)

/* set_bit uses setb, as gas doesn't recognize setc */
/* set_bit()使用了setb 指令，因为汇编编译器gas 不能识别指令setc */
//// 测试指定位偏移处比特位的值(0 或1)，并返回该比特位值。(应该取名为test_bit()更妥帖)
mount_root
  