/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <kestrel/kernel.h>
#include <kestrel/errno.h>

unsigned long int saved_mem_lower;
unsigned long int saved_mmap_addr;
unsigned long int saved_mmap_length;

unsigned long int extended_memory;
unsigned long int init_free_mem_start;

static void *heap = NULL;

static void *last_address;
static void *managed_address_start;
static int allocater_is_inited = 0;

void *get_kernel_heap() {
	if(!heap) kernel_panic("kernel heap not initialized", EFAULT);
	return heap;
}

void init_memory() {
	unsigned long int count = 0;

	kernel_printf("Getting lower memory...");
	saved_mem_lower = get_memsize (0);
	kernel_printf(" %luKB\n", saved_mem_lower);

	kernel_printf("Getting upper memory...");
	saved_mem_upper = get_memsize (1);
	kernel_printf(" %luKB\n", saved_mem_upper);

	kernel_puts("Turning on gate A20...");
	if(!gateA20(1)) kernel_panic("failed to turn on gate A20", 0);
	kernel_putchar('\n');

	extended_memory = saved_mem_upper;

	unsigned long int addr = get_code_end();
	saved_mmap_addr = addr;
	saved_mmap_length = 0;

	kernel_puts("Getting E820 memory...");
	do {
		count = get_mmap_entry((void *)addr, count);
		if(!*(unsigned long int *)addr) break;
		saved_mmap_length += *(unsigned long int *)addr + 4;
		addr += *(unsigned long int *)addr + 4;
	} while(count);

	init_free_mem_start = addr;
	heap = ((char *)init_free_mem_start) + 256 * sizeof (char *);

	errno = 0;
	kernel_malloc_init(get_kernel_heap());
}

void kernel_malloc_init(void *start) {
	//开始管理内存首地址
	managed_address_start = last_address = start;
	allocater_is_inited = 1;
}

void *kernel_malloc(size_t numbytes) {
	void *current_location;
	void *memory_location;
	void *tmp_location;
	struct mem_control_block *tmp_location_mcb;
	struct mem_control_block *current_location_mcb;
	//如果首地址未被初始化，则执行kernel_malloc_init()
	if(!allocater_is_inited) {
		kernel_malloc_init(get_kernel_heap());
	}
	//应初始化的内存实际为numbytes+mcb控制块所占内存
	numbytes = numbytes + sizeof(struct mem_control_block);
	//用于下面判断是否改继续申请内存
	memory_location = 0;
	tmp_location = 0;
	current_location = managed_address_start;
	//如果当前内存偏移不等于当前内存断址，开始循环寻找可用内存
	while(current_location != last_address) {
		current_location_mcb = (struct mem_control_block *)current_location;
		if(current_location_mcb->is_available) { 
			//如果此块内存空间大小正好等于申请大小
			if(current_location_mcb->size == numbytes) {
				current_location_mcb->is_available = 0;
				memory_location = current_location;
				break;
				//如果大小不够，尝试进行内存整理
			} else if(current_location_mcb->size < numbytes) {
				tmp_location = (char *)current_location + current_location_mcb->size;
				tmp_location_mcb = (struct mem_control_block*)tmp_location;
				//判断此mcb是否为内存断址
				if(tmp_location >= last_address) {
					//放弃此段内存空间，向下寻找合适内存空间
					current_location = (char *)current_location + current_location_mcb->size;
					break;
					//如果下一段内存空间被占用，放弃使用此块内存
				} else if(!tmp_location_mcb->is_available) {
					//放弃此段内存空间，向下寻找合适内存空间
					current_location = (char *)current_location + current_location_mcb->size;
					break;
				} else {
					//合并相邻内存空间，使其成为一块大的内存空间
					current_location_mcb->size += tmp_location_mcb->size;
					continue;
				}
			} else {
				//如果内存空间大于申请空间，对此区域空间进行分割
				current_location_mcb->is_available = 0;
				//分割内存空间，重建mcb
				tmp_location_mcb = (struct mem_control_block*)current_location + numbytes;
				tmp_location_mcb->size = current_location_mcb->size - numbytes;
				tmp_location_mcb->is_available = 1;
				current_location_mcb->size = numbytes;
				//----------内存空间分割完成----------//
				tmp_location = (char *)current_location + current_location_mcb->size;
				tmp_location_mcb = (struct mem_control_block*)tmp_location;
				memory_location = current_location;
				break;
			}

		}
		//放弃此段内存空间，向下寻找合适内存空间
		current_location = (char *)current_location + current_location_mcb->size;
	}

	if(!memory_location) {
		memory_location = last_address;		//取得地址
		last_address = (char *)last_address + numbytes;
		current_location_mcb = (struct mem_control_block *)memory_location;		//准备写入内存控制块(MCB)
		current_location_mcb->is_available = 0;		//写入控制块
		current_location_mcb->size = numbytes;
	}
	//将指针偏移移动过控制块(MCB)
	memory_location = (char *)memory_location + sizeof(struct mem_control_block);

	errno = 0;

	//返回指针
	return memory_location;
}

void kernel_free(void *firstbyte) {
	if(!firstbyte) return;
	if(!allocater_is_inited) panic("free: allocater has not inited");
	struct mem_control_block *mcb;
	//回退到控制块
	mcb = (struct mem_control_block *)((char *)firstbyte - sizeof(struct mem_control_block));
	errno = 0;
	//判断地址合法性
	if(mcb->is_available) {
		kernel_printf("free: In address 0x%lx\n", (long int)firstbyte);
		errno = EFAULT;
		if(mcb->is_available > 1) panic("free: Invalid address!");
		kernel_puts("Warning: Double free!!!");
		errno = EFAULT;
	}
	//Woo!! 成功释放内存，啦啦啦~~~
	mcb->is_available = 1;
}

