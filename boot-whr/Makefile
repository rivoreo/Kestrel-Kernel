AS = as86 -0
LD = ld86 -0

ASFLAGS = -a
LDFLAGS = -s

sector:	a.out
	dd if=$^ of=$@ bs=32 skip=1

a.out:	sector.o
	$(LD) $(LDFLAGS) $^ -o $@

sector.o:	sector.s
	$(AS) $(ASFLAGS) $^ -o $@

clean:
	rm -f *.o a.out sector
