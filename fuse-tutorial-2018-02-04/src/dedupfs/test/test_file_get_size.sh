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


