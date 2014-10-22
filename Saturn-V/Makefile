export ARCH = x86
export ECHO = $(PWD)/tools/echo
export OS_NAME = $(shell uname -o)

first:	$(ECHO) kestrel

kestrel:	S-IC/start.com S-II/kernel.com
	$(ECHO) -ne "\000\000\000\000\000\000\000\000\000\000\000\000" | cat $^ - > $@

S-IC/start.com:	S-II/kernel.com
	make -C S-IC

S-II/kernel.com:	lib/libmstr.a
	make -C S-II

lib/libmstr.a:
	make -C lib

clean:
	make -C S-IC $@
	make -C S-II $@
	make -C lib $@

distclean:	clean
	rm -f $(ECHO) kestrel
