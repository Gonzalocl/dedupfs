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


bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba || exit_error

bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 0 aabbaabbaabbaabbaabbaabbaabbaabb || exit_error
test=$(hexdump bin/fs/files/test_file | tr -d " ")
ok=$(echo "0000000 0000 0000 0000 0000 bbaa bbaa bbaa bbaa
0000010 bbaa bbaa bbaa bbaa
0000018" | tr -d " ")
check "$test" "$ok"


bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 ccddccddccddccddccddccddccddccdd || exit_error
test=$(hexdump bin/fs/files/test_file | tr -d " ")
ok=$(echo "0000000 0000 0000 0000 0000 bbaa bbaa bbaa bbaa
0000010 bbaa bbaa bbaa bbaa ddcc ddcc ddcc ddcc
0000020 ddcc ddcc ddcc ddcc
0000028" | tr -d " ")
check "$test" "$ok"

