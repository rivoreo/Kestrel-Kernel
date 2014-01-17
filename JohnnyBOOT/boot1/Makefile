AS = as86 -0
LD = ld86 -0

ASFLAGS = -a
LDFLAGS = -s

sector: bootsector fdd
	dd if=bootsector of=$@ bs=32 skip=1

bootsector: bootsector.o
	$(LD) $(LDFLAGS) -o $@ $^

bootsector.o: bootsector.s
	$(AS) $(ASFLAGS) -o $@ $^
clean:
	rm -f *.o
	rm bootsector
	rm sector

