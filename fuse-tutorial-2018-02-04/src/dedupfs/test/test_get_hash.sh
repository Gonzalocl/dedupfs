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

rm -rf bin/fs/*
make


temp_file=$(mktemp)

data_size=1024
dd if=/dev/urandom bs=1 count=$data_size of=$temp_file 2> /dev/null
data=$(hexdump -v -e '/1 "%02x"' $temp_file)
test=$(bin/test_get_hash 1 $data_size $data)  || exit_error
ok=$(md5sum $temp_file | cut -d ' ' -f 1)
check "$test" "$ok"


data_size=1025
dd if=/dev/urandom bs=1 count=$data_size of=$temp_file 2> /dev/null
data=$(hexdump -v -e '/1 "%02x"' $temp_file)
test=$(bin/test_get_hash 1 $data_size $data)  || exit_error
ok=$(md5sum $temp_file | cut -d ' ' -f 1)
check "$test" "$ok"


data_size=1024
dd if=/dev/urandom bs=1 count=$data_size of=$temp_file 2> /dev/null
data=$(hexdump -v -e '/1 "%02x"' $temp_file)
test=$(bin/test_get_hash 2 $data_size $data)  || exit_error
ok=$(sha1sum $temp_file | cut -d ' ' -f 1)
check "$test" "$ok"


data_size=1025
dd if=/dev/urandom bs=1 count=$data_size of=$temp_file 2> /dev/null
data=$(hexdump -v -e '/1 "%02x"' $temp_file)
test=$(bin/test_get_hash 2 $data_size $data)  || exit_error
ok=$(sha1sum $temp_file | cut -d ' ' -f 1)
check "$test" "$ok"

rm -f $temp_file