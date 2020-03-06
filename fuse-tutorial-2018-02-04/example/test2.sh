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


"$fs_bin" -s --block-size 2048 "$test_root_dir" "$test_mount_dir"


ref_etc_size=$(du -s "$ref_etc" | cut -f 1)
root_dir_initial_size=$(du -s "$test_root_dir" | cut -f 1)
root_dir_size_table="etc,size,increase,ratio"

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

function f_div {
  python -c "print('%.2f' % ($1/$2))"
}

steps=10
last_root_dir_size="$root_dir_initial_size"
for i in $(seq 1 $steps); do
  run_ref_test "etc$i" cp -a "$ref_etc" "$mount_dir/etc$i"
  run_ref_test "diff$i" diff -r "$ref_etc" "$mount_dir/etc$i"
  run_ref_test "du$i" du -s "$mount_dir/etc$i"

  root_dir_size=$(du -s "$test_root_dir" | cut -f 1)
  increase="$(f_div $root_dir_size $last_root_dir_size)"
  ratio="$(f_div $root_dir_size $((ref_etc_size*i)))"
  root_dir_size_table="$root_dir_size_table
  $i,$root_dir_size,$increase,$ratio"

  last_root_dir_size="$root_dir_size"
done

for i in $(seq $steps -1 1); do
  run_ref_test "rm$i" rm -rf "$mount_dir/etc$i"

  etc_folders="$((i-1))"
  root_dir_size=$(du -s "$test_root_dir" | cut -f 1)
  increase="$(f_div $root_dir_size $last_root_dir_size)"
  ratio="$(f_div $root_dir_size $((ref_etc_size*etc_folders)))"
  root_dir_size_table="$root_dir_size_table
  $etc_folders,$root_dir_size,$increase,$ratio"

  last_root_dir_size="$root_dir_size"
done

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

root_dir_final_size=$(du -s "$test_root_dir" | cut -f 1)


echo "/etc folder size: $ref_etc_size"
echo "Initial root_dir size: $root_dir_initial_size"
echo "Final root_dir size: $root_dir_final_size"
echo "$root_dir_size_table" | column -s ',' -t

sleep 1
fusermount -u "$test_mount_dir"

rm -rf "$test_ws"
rm -rf "$ref_ws"