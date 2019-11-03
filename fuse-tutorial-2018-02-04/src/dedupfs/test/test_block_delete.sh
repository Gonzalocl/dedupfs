#!/bin/bash



make clean
make

bin/test_block_create blo 5 1 16 1 2 4 00112233445566778899aabbccddeeff 0123456789abcdef9876543210fedcba
hexdump -C bin/fs/blo/0011/2233445566778899aabbccddeeff

bin/test_block_create blo 5 1 16 1 2 4 00112233445566778899aabbccddeeff 0123456789abcdef9876543210fedcba
hexdump -C bin/fs/blo/0011/2233445566778899aabbccddeeff

bin/test_block_delete blo 5 1 16 1 2 4 00112233445566778899aabbccddeeff 0123456789abcdef9876543210fedcba
hexdump -C bin/fs/blo/0011/2233445566778899aabbccddeeff

bin/test_block_delete blo 5 1 16 1 2 4 00112233445566778899aabbccddeeff 0123456789abcdef9876543210fedcba
hexdump -C bin/fs/blo/0011/2233445566778899aabbccddeeff


