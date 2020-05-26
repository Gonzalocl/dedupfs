#!/bin/bash

cd ..
make clean
make
cd example
rm -rf rootdir/*
../src/bbfs -s rootdir/ mountdir/

dd if=/dev/urandom of=mountdir/03 bs=1 count=3
for i in $(seq 1 10) ; do
    echo $i
    dd if=/dev/urandom of=mountdir/$i bs=1 count=$i
done
ls -l mountdir

fusermount -u mountdir

