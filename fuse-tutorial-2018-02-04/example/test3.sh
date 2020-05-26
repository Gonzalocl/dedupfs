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
    echo ../src/bbfs "$3" --block-size "$4" "$1" "$2"
  else
    echo ../src/bbfs "$3" "$1" "$2"
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


