#!/bin/bash

COLOR_LOG="\e[36m"
COLOR_ERR="\e[31m"
COLOR_RESET="\e[0m"

rm -rf bin/fs/*
make


bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 0 aabbaabbaabbaabbaabbaabbaabbaabb || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )

ok=$(echo "0000000 0000 0000 0000 0000 bbaa bbaa bbaa bbaa
0000010 bbaa bbaa bbaa bbaa
0000018" | tr -d " ")
test=$(hexdump bin/fs/files/test_file | tr -d " ")

if [[ "$test" = "$ok" ]]; then
    echo -e "${COLOR_LOG}$0 OK${COLOR_RESET}"
else
    echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1
fi



bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 ccddccddccddccddccddccddccddccdd || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )

ok=$(echo "0000000 0000 0000 0000 0000 bbaa bbaa bbaa bbaa
0000010 bbaa bbaa bbaa bbaa ddcc ddcc ddcc ddcc
0000020 ddcc ddcc ddcc ddcc
0000028" | tr -d " ")
test=$(hexdump bin/fs/files/test_file | tr -d " ")

if [[ "$test" = "$ok" ]]; then
    echo -e "${COLOR_LOG}$0 OK${COLOR_RESET}"
else
    echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1
fi

