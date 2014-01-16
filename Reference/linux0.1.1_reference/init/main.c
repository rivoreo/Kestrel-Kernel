//#include <linux/kernel.h>

int printk (const char *fmt, ...);

void main(void) {
	printk("Hello Kernel World!\n");
}
