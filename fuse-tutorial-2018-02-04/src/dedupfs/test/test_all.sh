#!/bin/bash


tests="test_file_set_block_hash.sh
test_file_get_block_hash.sh
test_file_set_size.sh
test_file_get_size.sh
test_get_hash.sh"


make clean
make

for test in $tests ; do
    ./$test
done


