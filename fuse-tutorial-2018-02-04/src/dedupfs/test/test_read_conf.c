#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "../block_handler.h"

#define CONF_FILENAME "conf"
struct block_handler_conf* handler_conf = NULL;

int read_conf_file(char *blocks_path) {

    // TODO use open read write?
    char path[PATH_MAX];
    int path_length;
    int ret_value = 0;


    if (snprintf(path, PATH_MAX, "%s/%s", blocks_path, CONF_FILENAME) < 0) {
        return -EIO;
    }

    FILE * conf_file;
    if ((conf_file = fopen(path, "rb")) == NULL) {
        return -errno;
    }

    // TODO check this can be negative
    if (fread(&path_length, 1, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (fread(handler_conf->blocks_path, 1, path_length, conf_file) != path_length) {
        ret_value = -EIO;
        goto out;
    }

    if (fread(&handler_conf->block_size, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (handler_conf->block_size <= 0) {
        ret_value = -ENOTRECOVERABLE;
        goto out;
    }

    // TODO check read values
    if (fread(&handler_conf->hash_type, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (fread(&handler_conf->hash_length, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (fread(&handler_conf->hash_split, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (fread(&handler_conf->bytes_link_counter, 4, 1, conf_file) != 1) {
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
    handler_conf->blocks_path = malloc(4096);
    int ret = read_conf_file("bin");

    printf("ret = %d\n", ret);
    printf("\n");
    printf("blocks_path:        %s\n", handler_conf->blocks_path);
    printf("block_size:         %d\n", handler_conf->block_size);
    printf("hash_type:          %d\n", handler_conf->hash_type);
    printf("hash_length:        %d\n", handler_conf->hash_length);
    printf("hash_split:         %d\n", handler_conf->hash_split);
    printf("bytes_link_counter: %d\n", handler_conf->bytes_link_counter);



    return 0;
}


