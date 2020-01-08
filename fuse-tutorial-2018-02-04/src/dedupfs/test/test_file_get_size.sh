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
test=$(bin/test_file_get_size bin/fs files 1 blocks 5 1 2 /test_file) || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
ok="789"
check "$test" "$ok"


bin/test_file_set_size bin/fs files 1 blocks 5 1 2 /test_file 790 || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
test=$(bin/test_file_get_size bin/fs files 1 blocks 5 1 2 /test_file) || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
ok="790"
check "$test" "$ok"


bin/test_file_set_size bin/fs files 1 blocks 5 1 2 /test_file 999999 || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
test=$(bin/test_file_get_size bin/fs files 1 blocks 5 1 2 /test_file) || ( echo -e "${COLOR_ERR}$0 FAILED${COLOR_RESET}" && exit 1 )
ok="999999"
check "$test" "$ok"




