#!/bin/bash

COLOR_LOG="\e[36m"
COLOR_ERR="\e[31m"
COLOR_RESET="\e[0m"

function check {

if [[ "$1" = "$2" ]]; then
    echo -e "${COLOR_LOG}$0 OK${COLOR_RESET}"
else
    echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1
fi

}

function exit_error {
    echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1
}

fs_path="bin/fs"

rm -rf $fs_path/*
make


files_path="files"
hash_type=1
hash_command="md5sum"
blocks_path="blocks"
block_size=5
hash_split=1
hash_split_size=2
path="/test_file"


bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error

file_size=789
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
test=$(bin/test_file_get_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path) || exit_error
ok="$file_size"
check "$test" "$ok"

file_size=790
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
test=$(bin/test_file_get_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path) || exit_error
ok="$file_size"
check "$test" "$ok"

file_size=999999
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
test=$(bin/test_file_get_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path) || exit_error
ok="$file_size"
check "$test" "$ok"






file_size=$(($block_size*3))
hash0=$(dd if=/dev/urandom bs=1 count=$block_size 2> /dev/null | $hash_command | cut -d ' ' -f 1)
hash1=$(dd if=/dev/urandom bs=1 count=$block_size 2> /dev/null | $hash_command | cut -d ' ' -f 1)
hash2=$(dd if=/dev/urandom bs=1 count=$block_size 2> /dev/null | $hash_command | cut -d ' ' -f 1)

bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
block=2
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block $hash2 || exit_error
block=1
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block $hash1 || exit_error
block=0
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block $hash0 || exit_error


test=$(bin/test_file_get_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path) || exit_error
ok="$file_size"
check "$test" "$ok"

block=0
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error
ok="$hash0"
check "$test" "$ok"

block=1
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error
ok="$hash1"
check "$test" "$ok"

block=2
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error
ok="$hash2"
check "$test" "$ok"


test=$(hexdump -v -e '/1 "%02x"' $fs_path/$files_path/$path)
ok="$(printf '%02x' $file_size)00000000000000$hash0$hash1$hash2"
check "$test" "$ok"
