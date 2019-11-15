#!/bin/bash



make clean
make


bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_filerrrrrrrrrrr 0123456789abcdef9876543210fedcba
bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba


