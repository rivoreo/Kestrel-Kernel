#!/bin/sh

setsid qemu 1 -no-reboot -no-shutdown -s < /dev/null &
gdb --command=gdbcommands

