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

function f_div {
  python -c "print('%.5f' % ($1/$2)) if $2 else print('inf')"
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

fs_mount "$test_root_dir" "$test_mount_dir" -s

steps=10
# test dedup average case /etc
ref_etc="$com_ws/etc"
cp -a /etc "$ref_etc" > /dev/null 2> /dev/null

ref_etc_size=$(du -sb "$ref_etc" | cut -f 1)
root_dir_initial_size=$(du -sb "$test_root_dir" | cut -f 1)
root_dir_size_table="etc copies,root dir size,root dir increase,root dir ratio relative to reference etc"

last_root_dir_size="$root_dir_initial_size"
for i in $(seq 1 $steps); do
  run_ref_test "cp -a $ref_etc $mount_dir/etc$i"
  run_ref_test "diff -r $ref_etc $mount_dir/etc$i"

  root_dir_size=$(du -sb "$test_root_dir" | cut -f 1)
  increase="$(f_div $root_dir_size $last_root_dir_size)"
  ratio="$(f_div $root_dir_size $((ref_etc_size*i)))"
  root_dir_size_table="$root_dir_size_table\n$i,$root_dir_size,$increase,$ratio"

  last_root_dir_size="$root_dir_size"
done

for i in $(seq $steps -1 1); do
  run_ref_test "rm -rf $mount_dir/etc$i"
  run_ref_test "diff -r $ref_etc $mount_dir"

  etc_folders="$((i-1))"
  root_dir_size=$(du -sb "$test_root_dir" | cut -f 1)
  increase="$(f_div $root_dir_size $last_root_dir_size)"
  ratio="$(f_div $root_dir_size $((ref_etc_size*etc_folders)))"
  root_dir_size_table="$root_dir_size_table\n$etc_folders,$root_dir_size,$increase,$ratio"

  last_root_dir_size="$root_dir_size"
done

root_dir_final_size=$(du -sb "$test_root_dir" | cut -f 1)

echo -e "$result_all"
echo "Initial root_dir size: $root_dir_initial_size"
echo "Final root_dir size: $root_dir_final_size"
echo "Reference etc folder size: $ref_etc_size"
echo "Average dedup test results"
echo -e "$root_dir_size_table" | column -s ',' -t
echo "ENTER to clean"
read l
#tail -n +1 tmp.test.*/0* | less
#tail -n +1 tmp.ref.*/0* | less

fs_umount "$test_mount_dir"

rm -rf "$test_ws"
rm -rf "$ref_ws"
rm -rf "$com_ws"
