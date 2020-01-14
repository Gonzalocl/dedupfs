#!/bin/bash


tests="test_block_create.sh
test_file_set_block_hash.sh
test_file_get_block_hash.sh
test_file_set_size.sh
test_file_get_size.sh
test_get_hash.sh
test_cache_write.sh
test_cache_read.sh"

mkdir bin
make clean
make

for test in $tests ; do
    ./$test
done


