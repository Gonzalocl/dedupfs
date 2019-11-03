#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../block_handler.h"

int main (int argc, char *argv[]) {

    if (argc != 10) {
        printf("ERROR\n");
        return 1;
    }

    struct block_handler_conf conf;
    strcpy(conf.blocks_path, argv[1]);
    conf.block_size = atoi(argv[2]);
    conf.hash_type = atoi(argv[3]);
    conf.hash_length = atoi(argv[4]);
    conf.hash_split = atoi(argv[5]);
    conf.hash_split_size = atoi(argv[6]);
    conf.bytes_link_counter = atoi(argv[7]);
    unsigned char hash[conf.hash_length];
    unsigned char data[conf.block_size];

    for (int i = 0; i < conf.hash_length; i++) {
        sscanf(argv[8] + i*2, "%02hhx", &hash[i]);
    }

    strncpy(data, argv[9], conf.block_size);

    int ret;
    ret = block_handler_init("bin/fs", &conf);
    printf("init: %d\n", ret);
    ret = block_create(hash, data);
    printf("create: %d\n", ret);
//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}



