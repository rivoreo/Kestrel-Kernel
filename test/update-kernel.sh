#!/bin/sh

[ `id -u` != "0" ] && exec sudo "$0" "$@"

[ -z "$1" ] && HD=1 || HD="$1"
#set -e
kpartx -av "$HD" || exit
#RESULT=`kpartx -av "$HD"`
# TODO: Using RESULT
P=loop0p1
mount /dev/mapper/$P /mnt || exit
cp /home/whr/src/kestrel/Saturn-V/kestrel /mnt/
umount /mnt
kpartx -d "$HD"
