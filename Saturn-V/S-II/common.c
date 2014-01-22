#include "shared.h"

unsigned long int saved_mem_lower;
unsigned long int saved_mmap_addr;
unsigned long int saved_mmap_length;

unsigned long int extended_memory;
unsigned long int init_free_mem_start;

unsigned long int force_cdrom_as_boot_device = 1;

int errno = 0;

void init_memory() {
	kernel_printf("Getting lower memory...");
	saved_mem_lower = get_memsize (0);
	kernel_printf(" %lu\n", saved_mem_lower);

	kernel_printf("Getting upper memory...");
	saved_mem_upper = get_memsize (1);
	kernel_printf(" %lu\n", saved_mem_upper);
}
