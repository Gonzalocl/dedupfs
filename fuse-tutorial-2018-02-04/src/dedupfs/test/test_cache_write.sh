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
block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
file_size=$block_size
write_data_size=$(($block_size-2))
write_data_offset=0
write_data=$(mktemp)
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))


bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block $block_hash || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_offset=1
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_offset=2
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_size=$block_size
write_data_offset=0
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error



rm -f $block_data $write_data
rm -rf $fs_path/*















block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
file_size=$(($block_size*2))
write_data_size=$(($block_size-2))
write_data_offset=$(($block_size+0))
write_data=$(mktemp)
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))


bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block $block_hash || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_offset=$(($block_size+1))
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_offset=$(($block_size+2))
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error





block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
write_data_size=$block_size
write_data_offset=$(($block_size+0))
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error
test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $block null) || exit_error

dd if=$write_data of=$block_data conv=notrunc bs=1 count=$write_data_size seek=$(($write_data_offset%$block_size)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"

ls $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $block_hash) 2>/dev/null > /dev/null && exit_error



rm -f $block_data $write_data
rm -rf $fs_path/*













block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
file_size=$(($block_size*3))
write_data_size=$(($block_size*3))
write_data_offset=0
write_data=$(mktemp)
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))





bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+0)) $block_hash || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+1)) $block_hash || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+2)) $block_hash || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+0)) null) || exit_error
dd if=$write_data of=$block_data bs=1 count=$block_size skip=$(($block_size*0)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+1)) null) || exit_error
dd if=$write_data of=$block_data bs=1 count=$block_size skip=$(($block_size*1)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+2)) null) || exit_error
dd if=$write_data of=$block_data bs=1 count=$block_size skip=$(($block_size*2)) 2> /dev/null
ok=$($hash_command $block_data | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(hexdump -v -e '/1 "%02x"' $block_data)01000000
check "$test" "$ok"


rm -f $block_data $write_data
rm -rf $fs_path/*













block_data=$(mktemp)
dd if=/dev/urandom of=$block_data bs=1 count=$block_size 2> /dev/null
block_hash=$($hash_command $block_data | cut -d ' ' -f 1)
file_size=$(($block_size*3))
write_data_size=$(($block_size*3 - 2))
write_data_offset=0
write_data=$(mktemp)
dd if=/dev/urandom of=$write_data bs=1 count=$write_data_size 2> /dev/null
block=$(($write_data_offset/$block_size))





bin/test_file_mknod $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null || exit_error
bin/test_file_set_size $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $file_size || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+0)) $block_hash || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+1)) $block_hash || exit_error
bin/test_file_set_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+2)) $block_hash || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error
bin/test_block_create $fs_path $blocks_path $block_size $hash_type $hash_length $hash_split $hash_split_size $bytes_link_counter $block_hash $(hexdump -v -e '/1 "%02x"' $block_data) || exit_error

bin/test_cache_write $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path $write_data_size $write_data_offset $(hexdump -v -e '/1 "%02x"' $write_data) || exit_error


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+0)) null) || exit_error
ok=$(dd if=$write_data bs=1 count=$(($block_size-$write_data_offset)) conv=notrunc seek=$write_data_offset 2> /dev/null | $hash_command | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(dd if=$write_data bs=1 count=$(($block_size-$write_data_offset)) conv=notrunc seek=$write_data_offset 2> /dev/null | hexdump -v -e '/1 "%02x"')01000000
check "$test" "$ok"


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+1)) null) || exit_error
ok=$(dd if=$write_data bs=1 count=$block_size conv=notrunc skip=$(($block_size-$write_data_offset)) 2> /dev/null | $hash_command | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(dd if=$write_data bs=1 count=$block_size conv=notrunc skip=$(($block_size-$write_data_offset)) 2> /dev/null | hexdump -v -e '/1 "%02x"')01000000
check "$test" "$ok"


test=$(bin/test_file_get_block_hash $fs_path $files_path $hash_type $blocks_path $block_size $hash_split $hash_split_size $path null $(($block+2)) null) || exit_error
ok=$(dd if=$write_data bs=1 count=$(($write_data_size-2*$block_size+$write_data_offset)) conv=notrunc skip=$((2*$block_size-$write_data_offset)) 2> /dev/null | $hash_command | cut -d ' ' -f 1)
check "$test" "$ok"

test=$(hexdump -v -e '/1 "%02x"' $fs_path/$blocks_path/$(get_path $hash_split $hash_split_size $test))
ok=$(dd if=$write_data bs=1 count=$(($write_data_size-2*$block_size+$write_data_offset)) conv=notrunc skip=$((2*$block_size-$write_data_offset)) 2> /dev/null | hexdump -v -e '/1 "%02x"')01000000
check "$test" "$ok"


rm -f $block_data $write_data
