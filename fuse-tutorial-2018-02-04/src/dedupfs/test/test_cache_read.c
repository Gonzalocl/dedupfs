#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../file_handler.h"
#include "../block_cache.h"

int main (int argc, char *argv[]) {

    if (argc != 11) {
        fprintf(stderr, "ERROR:%s:%d param\n", __FILE__, __LINE__);
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

    int data_size = atoi(argv[9]);
    int offset = atoi(argv[10]);
    char data[data_size];
    memset(data, 0, data_size);

    int ret;
    if ((ret = file_handler_init(&conf)) < 0) {
        fprintf(stderr, "ERROR:%s:%d file_handler_init: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

    int fd;
    if ((fd = file_open(&conf, path, 0)) < 0) {
        fprintf(stderr, "ERROR:%s:%d file_open: %d\n", __FILE__, __LINE__, fd);
        return EXIT_FAILURE;
    }

    struct block_cache *cache;
    if ((cache = cache_init(fd, 0, &conf)) == NULL) {
        fprintf(stderr, "ERROR:%s:%d cache_init: %p\n", __FILE__, __LINE__, cache);
        return EXIT_FAILURE;
    }

    if ((ret = cache_read(cache, data, data_size, offset)) < 0) {
        fprintf(stderr, "ERROR:%s:%d cache_write: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < data_size; i++) {
        printf("%02hhx", data[i]);
    }

//    fprintf(stderr, "%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return EXIT_SUCCESS;
}


