#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../file_handler.h"


int main (int argc, char *argv[]) {

    if (argc != 10) {
        printf("ERROR\n");
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

    int ret;
    if ((ret = file_handler_init(&conf)) < 0) {
        printf("ERROR: file_handler_init: %d\n", ret);
        return EXIT_FAILURE;
    }

    struct stat stat_buf;
    memset(&stat_buf, 0, sizeof(stat_buf));
    if ((ret = file_getattr(&conf, path, &stat_buf)) < 0) {
        printf("ERROR: file_getattr: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("st_size    = %ld\n", stat_buf.st_size);
    printf("st_blksize = %ld\n", stat_buf.st_blksize);
    printf("st_blocks  = %ld\n", stat_buf.st_blocks);



//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return EXIT_SUCCESS;
}



