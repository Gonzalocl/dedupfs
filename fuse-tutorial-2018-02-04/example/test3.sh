#!/bin/bash


mount_dir="mount_dir"
root_dir="root_dir"
work_dir="$(pwd)"

# $1: root_dir
# $2: mount_dir
# $3: (optional) -s
# $4: (optional) block-size
function fs_mount {
  if [[ $# -eq 4 ]]; then
    ../src/bbfs "$3" --block-size "$4" "$1" "$2"
  else
    ../src/bbfs "$3" "$1" "$2"
  fi
}

# $1: mount_dir
function fs_umount {
  fusermount -u "$1"
}

# clean workspace and build
function ws_remake {
  rm -rf rootdir/*
  cd ..
  make clean 2> /dev/null > /dev/null
  make 2> /dev/null > /dev/null
  cd example
}

ws_remake

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

fs_mount "$test_root_dir" "$test_mount_dir" -s





sleep 1
fs_umount "$test_mount_dir"

rm -rf "$test_ws"
rm -rf "$ref_ws"
