#!/bin/bash

fs_bin="../src/bbfs"
mount_dir="mount_dir"
root_dir="root_dir"


# cleaning
rm -rf rootdir/*
cd ..
make clean
make
cd example


# test workspace
test_ws="$(mktemp -d -p .)"
test_mount_dir="$test_ws/$mount_dir"
test_root_dir="$test_ws/$root_dir"
mkdir -p "$test_mount_dir"
mkdir -p "$test_root_dir"

# reference workspace
ref_ws="$(mktemp -d -p .)"
ok_stdout="$ref_ws/ok_stdout"
ok_stderr="$ref_ws/ok_stderr"
test_stdout="$ref_ws/test_stdout"
test_stderr="$ref_ws/test_stderr"
mkfifo "$ok_stdout"
mkfifo "$ok_stderr"
mkfifo "$test_stdout"
mkfifo "$test_stderr"

ref_mount_dir="$ref_ws/$mount_dir"
mkdir -p "$ref_mount_dir"

ref_etc="$ref_ws/etc"
cp -a /etc "$ref_etc" > /dev/null 2> /dev/null

#../src/bbfs -s rootdir/ mountdir/
#
#cd mountdir/
#
#
#du -sh ../etc/
#du -sh ../rootdir/
#read l
#
#cp -a ../etc/ etc1
#diff -r ../etc/ etc1/
#du -sh ../rootdir/
#read l
#
#cp -a etc1/ etc2
#diff -r etc1/ etc2
#du -sh ../rootdir/
#read l
#
#cp -a etc2 etc3
#diff -r etc2 etc3
#du -sh ../rootdir/
#read l
#
#dd if=/dev/zero  of=zero bs=1024 count=100000
#ls -lh zero
#du -sh ../rootdir/
#read l
#
#yes > yes &
#pid=$!
#for i in {1..10}; do sleep 1; echo $i; done
#kill $pid
#ls -lh yes
#du -sh ../rootdir/
#read l
#
#cd ..
#fusermount -u mountdir

rm -rf "$test_ws"
rm -rf "$ref_ws"