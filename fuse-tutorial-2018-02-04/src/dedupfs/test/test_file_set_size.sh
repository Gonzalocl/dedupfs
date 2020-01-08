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

rm -rf bin/fs/*
make


bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )

bin/test_file_set_size bin/fs files 1 blocks 5 1 2 /test_file 789 || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
test=$(hexdump bin/fs/files/test_file | tr -d " ")
ok=$(echo "0000000 0315 0000 0000 0000
0000008" | tr -d " ")
check "$test" "$ok"


bin/test_file_set_size bin/fs files 1 blocks 5 1 2 /test_file 790 || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
test=$(hexdump bin/fs/files/test_file | tr -d " ")
ok=$(echo "0000000 0316 0000 0000 0000
0000008" | tr -d " ")
check "$test" "$ok"


bin/test_file_set_size bin/fs files 1 blocks 5 1 2 /test_file 999999 || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
test=$(hexdump bin/fs/files/test_file | tr -d " ")
ok=$(echo "0000000 423f 000f 0000 0000
0000008" | tr -d " ")
check "$test" "$ok"


