#include "shared.h"

void *heap;

unsigned long int saved_mem_lower;
unsigned long int saved_mmap_addr;
unsigned long int saved_mmap_length;

unsigned long int extended_memory;
unsigned long int init_free_mem_start;

unsigned long int force_cdrom_as_boot_device = 1;

int errno = 0;

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

	unsigned long int addr = get_code_end ();
	saved_mmap_addr = addr;
	saved_mmap_length = 0;

	kernel_puts("Getting E820 memory...");
	do {
		count = get_mmap_entry((void *)addr, count);
		if(!*(unsigned long int *)addr) break;
		saved_mmap_length += *(unsigned long int *)addr + 4;
		addr += *(unsigned long int *)addr + 4;
	} while(count);

/*
	if(saved_mmap_length) {
		unsigned long long int max_addr;
		saved_mem_lower = mmap_avail_at(0) >> 10;
		saved_mem_upper = mmap_avail_at(0x100000) >> 10;
	} else {
		kernel_puts("Getting E801 memory...");
	}*/

	init_free_mem_start = addr;
	heap = ((char *)init_free_mem_start) + 256 * sizeof (char *);
}
