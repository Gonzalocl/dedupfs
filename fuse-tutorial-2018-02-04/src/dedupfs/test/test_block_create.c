#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../block_handler.h"

int main (int argc, char *argv[]) {

    if (argc != 11) {
        fprintf(stderr, "ERROR:%s:%d param\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    char fs_path[PATH_MAX];
    strcpy(fs_path, argv[1]);

    struct block_handler_conf conf;
    strcpy(conf.blocks_path, argv[2]);
    conf.block_size = atoi(argv[3]);
    conf.hash_type = atoi(argv[4]);
    conf.hash_length = atoi(argv[5]);
    conf.hash_split = atoi(argv[6]);
    conf.hash_split_size = atoi(argv[7]);
    conf.bytes_link_counter = atoi(argv[8]);
    unsigned char hash[conf.hash_length];
    unsigned char data[conf.block_size];

    for (int i = 0; i < conf.hash_length; i++) {
        sscanf(argv[9] + i*2, "%02hhx", &hash[i]);
    }

    for (int i = 0; i < conf.block_size; i++) {
        sscanf(argv[10] + i*2, "%02hhx", &data[i]);
    }

    int ret;
    if ((ret = block_handler_init(fs_path, &conf)) < 0) {
        fprintf(stderr, "ERROR:%s:%d block_handler_init: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

    if ((ret = block_create(hash, data)) < 0) {
        fprintf(stderr, "ERROR:%s:%d block_create: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

//    fprintf(stderr, "%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return EXIT_SUCCESS;
}



