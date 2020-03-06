#!/bin/bash

fs_bin=$(realpath "../src/bbfs")
mount_dir="mount_dir"
root_dir="root_dir"
work_dir=$(pwd)

# cleaning
rm -rf rootdir/*
cd ..
make clean 2> /dev/null > /dev/null
make 2> /dev/null > /dev/null
cd example


# test workspace
test_ws="$(mktemp -d -p . 'tmp.test.XXXX')"
test_ws="$(realpath $test_ws)"

test_mount_dir="$test_ws/$mount_dir"
test_root_dir="$test_ws/$root_dir"
mkdir -p "$test_mount_dir"
mkdir -p "$test_root_dir"

# reference workspace
ref_ws="$(mktemp -d -p . 'tmp.ref.XXXX')"
ref_ws="$(realpath $ref_ws)"

ref_mount_dir="$ref_ws/$mount_dir"
mkdir -p "$ref_mount_dir"

ref_etc="$ref_ws/etc"
cp -a /etc "$ref_etc" > /dev/null 2> /dev/null


"$fs_bin" -s "$test_root_dir" "$test_mount_dir"


ref_etc_size=$(du -s "$ref_etc" | cut -f 1)
root_dir_initial_size=$(du -s "$test_root_dir" | cut -f 1)


function run_ref_test {
  id="$1"
  shift

  cd "$ref_ws"
  "$@" > "$ref_ws/$id.stdout" 2> "$ref_ws/$id.stderr"

  cd "$test_ws"
  "$@" > "$test_ws/$id.stdout" 2> "$test_ws/$id.stderr"

  echo "########################### STDOUT ###############################"
  diff "$ref_ws/$id.stdout" "$test_ws/$id.stdout"
  echo

  echo "########################### STDERR ###############################"
  diff "$ref_ws/$id.stderr" "$test_ws/$id.stderr"
  echo; echo; echo
}


#id="0"
#cp -a "$ref_etc" "$ref_mount_dir/etc$id" > "$ref_ws/etc$id.stdout" 2> "$ref_ws/etc$id.stderr"
#cp -a "$ref_etc" "$test_mount_dir/etc$id" > "$test_ws/etc$id.stdout" 2> "$test_ws/etc$id.stderr"

run_ref_test "etc0" cp -a "$ref_etc" "$mount_dir/etc0"
run_ref_test "diff0" diff -r "$ref_etc" "$mount_dir/etc0"

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
sleep 1
fusermount -u "$test_mount_dir"

#rm -rf "$test_ws"
#rm -rf "$ref_ws"