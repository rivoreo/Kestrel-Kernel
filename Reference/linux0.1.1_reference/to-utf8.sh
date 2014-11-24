#!/bin/sh

for i in `find *` ; do
	echo $i
	iconv -f gb18030 -t utf-8 $i > /tmp/iconv.tmp && mv /tmp/iconv.tmp $i || echo Failed on $i
done
