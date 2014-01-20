export ARCH = x86
export ECHO := $(PWD)/tools/echo

first:	$(ECHO) kestrel

kestrel:	loader/arch/$(ARCH)/start.com kernel/kernel.com
	$(ECHO) -ne "\000\000\000\000\000\000\000\000\000\000\000\000" | cat $^ - > $@

loader/arch/$(ARCH)/start.com:	kernel/kernel.com
	make -C loader/arch/$(ARCH)

kernel/kernel.com:
	make -C kernel

clean:
	make -C loader/arch/$(ARCH) $@
	make -C kernel $@

distclean:	clean
	rm -f kestrel
