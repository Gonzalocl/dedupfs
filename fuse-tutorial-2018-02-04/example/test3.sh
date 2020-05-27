#!/bin/bash

color_log="\e[32m"
color_err="\e[31m"
color_reset="\e[0m"
result_all="########################### ALL TESTS ###############################"

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

# clean workspace and build.
function ws_remake {
  rm -rf rootdir/*
  cd ..
  make clean 2> /dev/null > /dev/null
  make 2> /dev/null > /dev/null
  cd example
}


# Run command on reference workspace and test workspace and show
# differences in stdout and stderr.
# Relative paths are relative to $ref_ws when run on reference
# workspace and relative to $test_ws when run on test workspace.
# Absolute paths are absolute.
# $@: command
function run_ref_test {
  # TODO tmp folder for output and remove at the end
  id="$RANDOM"

  cd "$ref_ws"
  "$@" > "$ref_ws/$id.stdout" 2> "$ref_ws/$id.stderr"

  cd "$test_ws"
  "$@" > "$test_ws/$id.stdout" 2> "$test_ws/$id.stderr"

  result="${color_log}OK:   "

  diff_stdout="$(diff $ref_ws/$id.stdout $test_ws/$id.stdout)"
  if [[ $diff_stdout != "" ]]; then
    echo "########################### STDOUT ###############################"
    echo "$diff_stdout"
    echo "########################### STDOUT ###############################"
    echo
    result="${color_err}FAIL: "
  fi

  diff_stderr="$(diff $ref_ws/$id.stderr $test_ws/$id.stderr)"
  if [[ $diff_stderr != "" ]]; then
    echo "########################### STDERR ###############################"
    echo "$diff_stderr"
    echo "########################### STDERR ###############################"
    echo
    result="${color_err}FAIL: "
  fi

  result="${result}${@}${color_reset}"
  result_all="$result_all\n$result"
  echo -e "$result"

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

fs_mount "$test_root_dir" "$test_mount_dir" -s


# make a copy of /etc test
ref_etc="$ref_ws/etc"
cp -a /etc "$ref_etc" > /dev/null 2> /dev/null

run_ref_test cp -a "$ref_etc" "$mount_dir"
run_ref_test diff -r "$ref_etc" "$mount_dir/etc"


sleep 1
fs_umount "$test_mount_dir"

rm -rf "$test_ws"
rm -rf "$ref_ws"

echo -e "$result_all"
