/*
* linux/fs/pipe.c
*
* (C) 1991 Linus Torvalds
*/

#include <signal.h>		// 信号头文件。定义信号符号常量，信号结构以及信号操作函数原型。

#include <linux/sched.h>	// 调度程序头文件，定义了任务结构task_struct、初始任务0 的数据，
// 还有一些有关描述符参数设置和获取的嵌入式汇编函数宏语句。
#include <linux/mm.h> /* for get_free_page */	/* 使用其中的get_free_page */
// 内存管理头文件。含有页面大小定义和一些页面释放函数原型。
#include <asm/segment.h>	// 段操作头文件。定义了有关段寄存器操作的嵌入式汇编函数。

//// 管道读操作函数。
// 参数inode 是管道对应的i 节点，buf 是数据缓冲区指针，count 是读取的字节数。
int
read_pipe (struct m_inode *inode, char *buf, int count)
{
  int chars, size, read = 0;

// 若欲读取的字节计数值count 大于0，则循环执行以下操作。
  while (count > 0)
    {
// 若当前管道中没有数据(size=0)，则唤醒等待该节点的进程，如果已没有写管道者，则返回已读
// 字节数，退出。否则在该i 节点上睡眠，等待信息。
      while (!(size = PIPE_SIZE (*inode)))
	{
	  wake_up (&inode->i_wait);
	  管道缓冲区长度(PAGE_SIZE) 