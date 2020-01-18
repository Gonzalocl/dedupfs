#!/bin/bash

cd ..
make clean
make
cd example
rm -rf rootdir/*
../src/bbfs -s rootdir/ mountdir/

cd mountdir/


du -sh ../etc/
du -sh ../rootdir/
read l

cp -a ../etc/ etc1
diff -r ../etc/ etc1/
du -sh ../rootdir/
read l

cp -a etc1/ etc2
diff -r etc1/ etc2
du -sh ../rootdir/
read l

cp -a etc2 etc3
diff -r etc2 etc3
du -sh ../rootdir/
read l

dd if=/dev/zero  of=zero bs=1024 count=100000
ls -lh zero
du -sh ../rootdir/
read l

yes > yes &
pid=$!
for i in {1..10}; do sleep 1; echo $i; done
kill $pid
ls -lh yes
du -sh ../rootdir/
read l

cd ..
fusermount -u mountdir

