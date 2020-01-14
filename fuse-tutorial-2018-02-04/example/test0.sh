#!/bin/bash -x

cd ..
make clean
make
cd example
rm -rf rootdir/*
../src/bbfs -s rootdir/ mountdir/
ls -l
read l
echo hola > mountdir/gigago
cat mountdir/gigago
cat rootdir/files/gigago
read l
find rootdir

fusermount -u mountdir

