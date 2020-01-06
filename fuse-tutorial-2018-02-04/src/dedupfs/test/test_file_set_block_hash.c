#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../file_handler.h"


int main (int argc, char *argv[]) {

    if (argc != 12) {
        printf("ERROR:%s:%d param\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    struct file_handler_conf conf;
    strcpy(conf.fs_path, argv[1]);
    strcpy(conf.files_path, argv[2]);
    conf.hash_type = atoi(argv[3]);
    strcpy(conf.block_handler.blocks_path, argv[4]);
    conf.block_handler.block_size = atoi(argv[5]);
    conf.block_handler.hash_split = atoi(argv[6]);
    conf.block_handler.hash_split_size = atoi(argv[7]);

    char *path = argv[8];

    unsigned char data[conf.block_handler.block_size];
    strncpy(data, argv[9], conf.block_handler.block_size);

    long block = atol(argv[10]);


    int ret;
    if ((ret = file_handler_init(&conf)) < 0) {
        printf("ERROR:%s:%d file_handler_init: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

    unsigned char hash[conf.block_handler.hash_length];
    for (int i = 0; i < conf.block_handler.hash_length; i++) {
        sscanf(argv[11] + i*2, "%02hhx", &hash[i]);
    }

    int fd;
    if ((fd = file_open(&conf, path, 0)) < 0) {
        printf("ERROR:%s:%d file_open: %d\n", __FILE__, __LINE__, fd);
        return EXIT_FAILURE;
    }

    if ((ret = file_set_block_hash(&conf, fd, block, hash)) < 0) {
        printf("ERROR:%s:%d file_set_block_hash: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }



//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return EXIT_SUCCESS;
}



