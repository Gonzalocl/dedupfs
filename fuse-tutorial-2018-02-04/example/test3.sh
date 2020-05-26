#!/bin/bash

cd ..
make clean
make
cd example
rm -rf rootdir/*
../src/bbfs -s rootdir/ mountdir/


fusermount -u mountdir

