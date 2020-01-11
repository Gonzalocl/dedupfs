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



block_data="0123456789"
block_hash="00112233445566778899aabbccddeeff"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}01000000"
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}02000000"
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}03000000"
check "$test" "$ok"


block_data="9876543210"
block_hash="00112233445566778899aabbccddee00"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}01000000"
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}02000000"
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $block_data || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok="${block_data}03000000"
check "$test" "$ok"

#TODO test delete
#bin/test_block_delete blo 5 1 16 1 2 4 00112233445566778899aabbccddeeff 0123456789abcdef9876543210fedcba
#hexdump -C bin/fs/blo/0011/2233445566778899aabbccddeeff






block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)02000000
check "$test" "$ok"


dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)02000000
check "$test" "$ok"


rm -f $block_data