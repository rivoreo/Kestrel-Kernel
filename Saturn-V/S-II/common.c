#include "shared.h"

unsigned long int saved_mem_lower;
unsigned long int saved_mmap_addr;
unsigned long int saved_mmap_length;

unsigned long int extended_memory;
unsigned long int init_free_mem_start;

unsigned long int force_cdrom_as_boot_device = 1;

int errno = 0;
