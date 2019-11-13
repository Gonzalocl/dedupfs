#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "../block_handler.h"

extern struct block_handler_conf* handler_conf;
extern char fs_path[PATH_MAX];
extern int read_conf_file(char *blocks_path);


int main () {

    handler_conf = malloc(sizeof(struct block_handler_conf));
    strcpy(fs_path, "bin/fs");
    int ret = read_conf_file("test_blocks");

    printf("ret = %d\n", ret);
    printf("\n");
    printf("blocks_path:        %s\n", handler_conf->blocks_path);
    printf("block_size:         %d\n", handler_conf->block_size);
    printf("hash_type:          %d\n", handler_conf->hash_type);
    printf("hash_length:        %d\n", handler_conf->hash_length);
    printf("hash_split:         %d\n", handler_conf->hash_split);
    printf("hash_split_size     %d\n", handler_conf->hash_split_size);
    printf("bytes_link_counter: %d\n", handler_conf->bytes_link_counter);



    return 0;
}


