export ARCH = x86
export ECHO := $(PWD)/tools/echo

first:	$(ECHO) kestrel

kestrel:	S-IC/start.com S-II/kernel.com
	$(ECHO) -ne "\000\000\000\000\000\000\000\000\000\000\000\000" | cat $^ - > $@

S-IC/start.com:	S-II/kernel.com
	make -C S-IC

S-II/kernel.com:
	make -C S-II

clean:
	make -C S-IC $@
	make -C S-II $@

distclean:	clean
	rm -f $(ECHO) kestrel
