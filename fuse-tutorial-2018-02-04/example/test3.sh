#!/bin/bash

color_log="\e[32m"
color_err="\e[31m"
color_reset="\e[0m"
result_all="\n\n########################### ALL TESTS ###############################"

mount_dir="mount_dir"
root_dir="root_dir"
work_dir="$(pwd)"

test_count=0

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
  make 2> /dev/null > /dev/null || ( echo -e "${color_err}Compilation error${color_reset}" && exit 1 ) || exit
  cd example
}


# Run command on reference workspace and test workspace and show
# differences in stdout and stderr.
# Relative paths are relative to $ref_ws when run on reference
# workspace and relative to $test_ws when run on test workspace.
# Absolute paths are absolute.
# $1: command string, ", $, ` and \ need double escape
function run_ref_test {
  # TODO tmp folder for output and remove at the end
  id="$(printf %05d $test_count)_$(echo $1 | tr -d -c [:alnum:])"
  ((test_count++))

  cd "$ref_ws"
  bash -c "$1" > "$ref_ws/$id.stdout" 2> "$ref_ws/$id.stderr"

  cd "$test_ws"
  bash -c "$1" > "$test_ws/$id.stdout" 2> "$test_ws/$id.stderr"

  cd "$work_dir"

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

# common workspace
com_ws="$(mktemp -d -p . 'tmp.com.XXXX')"
com_ws="$(realpath $com_ws)"

# test workspace
test_ws="$(mktemp -d -p . 'tmp.test.XXXX')"
test_ws="$(realpath $test_ws)"

test_mount_dir="$test_ws/$mount_dir"
test_root_dir="$com_ws/$root_dir"
mkdir -p "$test_mount_dir"
mkdir -p "$test_root_dir"

# reference workspace
ref_ws="$(mktemp -d -p . 'tmp.ref.XXXX')"
ref_ws="$(realpath $ref_ws)"

ref_mount_dir="$ref_ws/$mount_dir"
mkdir -p -m 755 "$ref_mount_dir"

fs_mount "$test_root_dir" "$test_mount_dir" -s 4096


# test copy regular file with different permissions
echo AA > "$com_ws/a"
echo BB > "$com_ws/b"
echo CC > "$com_ws/c"
echo DDDDDDDDDDDDDDDDDDDD > "$com_ws/d"

run_ref_test "cp $com_ws/a $mount_dir"
run_ref_test "cp $com_ws/b $mount_dir"
run_ref_test "cp $com_ws/c $mount_dir"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir"
run_ref_test "shopt -s globstar; for i in $mount_dir/**/*; do hexdump -C \$i; done"

run_ref_test "rm -f $mount_dir/a"
run_ref_test "rm -f $mount_dir/b"
run_ref_test "rm -f $mount_dir/c"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir"

chmod 777 "$com_ws/a"
chmod 444 "$com_ws/b"
chmod 000 "$com_ws/c"

run_ref_test "cp $com_ws/a $mount_dir"
run_ref_test "cp $com_ws/b $mount_dir"
run_ref_test "cp $com_ws/c $mount_dir"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir"
run_ref_test "shopt -s globstar; for i in $mount_dir/**/*; do hexdump -C \$i; done"

run_ref_test "dd if=$com_ws/d of=$mount_dir/a status=noxfer"
run_ref_test "dd if=$com_ws/d of=$mount_dir/b status=noxfer"
run_ref_test "dd if=$com_ws/d of=$mount_dir/c status=noxfer"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir"
run_ref_test "shopt -s globstar; for i in $mount_dir/**/*; do hexdump -C \$i; done"

run_ref_test "rm -f $mount_dir/a"
run_ref_test "rm -f $mount_dir/b"
run_ref_test "rm -f $mount_dir/c"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir"

# test make a copy of /etc
ref_etc="$com_ws/etc"
cp -a /etc "$ref_etc" > /dev/null 2> /dev/null

run_ref_test "cp -a $ref_etc $mount_dir"
run_ref_test "find $mount_dir"
run_ref_test "ls -la $mount_dir/etc"
run_ref_test "diff -r $ref_etc $mount_dir/etc"
#run_ref_test "shopt -s globstar; for i in $mount_dir/**/*; do hexdump -C \$i; done"

# test add at the end
run_ref_test "echo a > $mount_dir/add0"
run_ref_test "ls -la $mount_dir"
run_ref_test "cat $mount_dir/add0"

run_ref_test "echo bb >> $mount_dir/add0"
run_ref_test "ls -la $mount_dir"
run_ref_test "cat $mount_dir/add0"

run_ref_test "echo a >> $mount_dir/add1"
run_ref_test "ls -la $mount_dir"
run_ref_test "cat $mount_dir/add1"

run_ref_test "echo bb >> $mount_dir/add1"
run_ref_test "ls -la $mount_dir"
run_ref_test "cat $mount_dir/add1"

echo -e "$result_all"
echo "ENTER to clean"
read l
#tail -n +1 tmp.test.*/0* | less
#tail -n +1 tmp.ref.*/0* | less

fs_umount "$test_mount_dir"

rm -rf "$test_ws"
rm -rf "$ref_ws"
rm -rf "$com_ws"
