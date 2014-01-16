#!/bin/sh

for i in `find *` ; do
	echo $i
	iconv -f gb18030 -t utf8 $i -o /tmp/iconv.tmp && mv /tmp/iconv.tmp $i || echo Failed on $i
done
