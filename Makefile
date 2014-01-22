export ARCH = x86
export ECHO := $(PWD)/tools/echo

first:	$(ECHO) kestrel

kestrel:	loader/start.com kernel/kernel.com
	$(ECHO) -ne "\000\000\000\000\000\000\000\000\000\000\000\000" | cat $^ - > $@

loader/start.com:	kernel/kernel.com
	make -C loader

kernel/kernel.com:
	make -C kernel

clean:
	make -C loader $@
	make -C kernel $@

distclean:	clean
	rm -f $(ECHO) kestrel
