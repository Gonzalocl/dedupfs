#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "../block_handler.h"

#define CONF_FILENAME "conf"
struct block_handler_conf* handler_conf = NULL;
char fs_path[PATH_MAX];

int write_conf_file() {

    // TODO use open read write?
    char path[PATH_MAX];
    int path_length;
    int ret_value = 0;


    if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, handler_conf->blocks_path, CONF_FILENAME) < 0) {
        return -EIO;
    }

    FILE * conf_file;
    if ((conf_file = fopen(path, "wb")) == NULL) {
        return -errno;
    }

    path_length = strnlen(handler_conf->blocks_path, PATH_MAX);
    if (fwrite(&path_length, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (fwrite(handler_conf->blocks_path, 1, path_length, conf_file) != path_length) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->block_size, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->hash_type, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->hash_length, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->hash_split, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->hash_split_size, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    if (fwrite(&handler_conf->bytes_link_counter, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }

    out:
    if (fclose(conf_file) != 0) {
        return -errno;
    }
    return ret_value;
}


int main () {

    handler_conf = malloc(sizeof(struct block_handler_conf));
    strcpy(handler_conf->blocks_path, "test_blocks");
    handler_conf->block_size = 25;
    handler_conf->hash_type = 6;
    handler_conf->hash_length = 256;
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
    printf("hash_split_size     %d\n", handler_conf->hash_split_size);
    printf("bytes_link_counter: %d\n", handler_conf->bytes_link_counter);



    return 0;
}


