#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "../block_handler.h"
#include "../block_handler.c"



int main () {

    handler_conf = malloc(sizeof(struct block_handler_conf));
    strcpy(handler_conf->blocks_path, "test_blocks");
    handler_conf->block_size = 25;
    handler_conf->hash_type = 1;
    handler_conf->hash_length = 16;
    handler_conf->hash_split = 1;
    handler_conf->hash_split_size = 2;
    handler_conf->bytes_link_counter = 4;
    strcpy(fs_path, "bin/fs");
    int ret = write_conf_file();

    printf("ret = %d\n", ret);
    printf("\n");
    printf("blocks_path:        %s\n", handler_conf->blocks_path);
    printf("block_size:         %d\n", handler_conf->block_size);
    printf("hash_type:          %d\n", handler_conf->hash_type);
    printf("hash_length:        %d\n", handler_conf->hash_length);
    printf("hash_split:         %d\n", handler_conf->hash_split);
    printf("hash_split_size:    %d\n", handler_conf->hash_split_size);
    printf("bytes_link_counter: %d\n", handler_conf->bytes_link_counter);



    return 0;
}


