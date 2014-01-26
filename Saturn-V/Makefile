export ARCH = x86
export ECHO := $(PWD)/tools/echo

first:	$(ECHO) kestrel

kestrel:	S-IC/start.com S-II/kernel.com
	$(ECHO) -ne "\000\000\000\000\000\000\000\000\000\000\000\000" | cat $^ - > $@

S-IC/start.com:	S-II/kernel.com
	make -C S-IC

S-II/kernel.com:	3part-lib/libmstr.a
	make -C S-II

3part-lib/libmstr.a:
	make -C 3part-lib

clean:
	make -C S-IC $@
	make -C S-II $@
	make -C 3part-lib $@

distclean:	clean
	rm -f $(ECHO) kestrel
