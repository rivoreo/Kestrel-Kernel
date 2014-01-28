/*	A part of kestrel
	Kestrel Kernel Project
	Copyright 2007-2014 BMY-Soft
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 */

#include <kestrel/kernel.h>
//#include <kestrel/types.h>
#include <kestrel/memory.h>

void *last_address;
void *managed_address_start;
int has_inited = 0;

void kernel_malloc_init() {
	//开始管理内存首地址
	last_address = (char *)heap+102400;
	managed_address_start = last_address;
	has_inited = 1;
}

void *kernel_malloc(size_t numbytes) {
	void *current_location;
	void *memory_location;
	struct mem_control_block *current_location_mcb;
	//如果首地址未被初始化，则执行kernel_malloc_init()
	if(!has_inited) {
		kernel_malloc_init();
	}
	//应初始化的内存实际为numbytes+mcb控制块所占内存
	numbytes = numbytes + sizeof(struct mem_control_block);
	//用于下面判断是否改继续申请内存
	memory_location = 0;
	current_location = managed_address_start;
/*	//如果当前内存偏移不等于当前内存断址，开始循环寻找可用内存*/
	while(current_location != last_address) {
		current_location_mcb = (struct mem_control_block *)current_location;
		if(current_location_mcb->is_available && 
		current_location_mcb->size >= numbytes) {
			current_location_mcb->is_available = 0;
			memory_location = current_location;
			break;
		}
		current_location = current_location + current_location_mcb->size;
	}

	if(!memory_location) {
		memory_location = last_address;
		//申请内存
		last_address = last_address + numbytes;
		//准备写入内存控制块(MCB)
		current_location_mcb = (struct mem_control_bloock *)memory_location;
		//写入控制块
		current_location_mcb->is_available = 0;
		current_location_mcb->size = numbytes;
	}
	//将指针偏移移动过控制块(MCB)
	memory_location = memory_location + sizeof(struct mem_control_block);
	//返回指针
	return memory_location;
}

void kernel_free(void *firstbyte) {
	if(!firstbyte) return;
	struct mem_control_block *mcb;
	//回退到控制块
	mcb = firstbyte - sizeof(struct mem_control_block);
	//判断地址合法性
	if(mcb->is_available) {
		if(mcb->is_available > 1) panic("free: Invalid address!");
		kernel_puts("Warning:Double free!!!");
	}
	//Woo!! 成功释放内存，啦啦啦~~~
	mcb->is_available = 1;
}

