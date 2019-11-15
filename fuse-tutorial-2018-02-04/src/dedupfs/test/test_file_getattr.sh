#!/bin/bash



make clean
make


bin/test_file_mknod bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x01\x00\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x04\x00\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x05\x00\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x06\x00\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\xFE\x01\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\xFF\x01\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x00\x02\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba

echo "###############################################"
echo -n -e '\x01\x02\x00\x00\x00\x00\x00\x00' > bin/fs/files/test_file
bin/test_file_getattr bin/fs files 1 blocks 5 1 2 /test_file 0123456789abcdef9876543210fedcba


make clean

