#!/bin/bash

COLOR_LOG="\e[36m"
COLOR_ERR="\e[31m"
COLOR_RESET="\e[0m"

function check {

if [[ "$1" = "$2" ]]; then
    echo -e "${COLOR_LOG}$0:${BASH_LINENO} OK${COLOR_RESET}"
else
    echo -e "${COLOR_ERR}$0:${BASH_LINENO} FAILED${COLOR_RESET}" && exit 1
fi

}

function exit_error {
    echo -e "${COLOR_ERR}$0:${BASH_LINENO} FAILED${COLOR_RESET}" && exit 1
}

function get_path {
    for i in $(seq 0 $(($1-1))) ; do
        echo -n ${3:$((i*$2*2)):$(($2*2))}"/"
    done
    echo ${3:$(($1*$2*2))}
}

fs_path="bin/fs"

rm -rf $fs_path/*
make


files_path="files"
hash_type=1
hash_command="md5sum"
hash_length=16
blocks_path="blocks"
block_size=5
hash_split=1
hash_split_size=2
bytes_link_counter=4
path="/test_file"


file_size=$(($block_size*5 - 2))


#initial blocks
block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)

bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error

for b in $(seq 0 $(($file_size/$block_size)) ) ; do
    bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null b $block_hash || exit_error
    bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
done


write_data=$(mktemp)
for s in $(seq $file_size) ; do
    echo "Test size: $s"
    for o in $(seq 0 $(($file_size-$s)) ) ; do
        write_data_size=$s
        write_data_offset=$o
        dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
        bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error

        test=$(bin/test_cache_read $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset) || exit_error
        ok=$(hexdump -v -e '/1 "%02x"' $write_data)
        check "$test" "$ok"

    done
done


rm -f $block_data $write_data

