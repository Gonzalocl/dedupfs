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
bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 ccddccddccddccddccddccddccddccdd || exit_error

test=$(bin/test_file_get_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 0 ccddccddccddccddccddccddccddccdd) || exit_error
ok="aabbaabbaabbaabbaabbaabbaabbaabb"
check "$test" "$ok"

test=$(bin/test_file_get_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 ccddccddccddccddccddccddccddccdd) || exit_error
ok="ccddccddccddccddccddccddccddccdd"
check "$test" "$ok"

bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 0 01010101010101010101010101010101 || exit_error
bin/test_file_set_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 95959595959595959595959595959595 || exit_error

test=$(bin/test_file_get_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 0 01010101010101010101010101010101) || exit_error
ok="01010101010101010101010101010101"
check "$test" "$ok"

test=$(bin/test_file_get_block_hash bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba 1 95959595959595959595959595959595) || exit_error
ok="95959595959595959595959595959595"
check "$test" "$ok"
