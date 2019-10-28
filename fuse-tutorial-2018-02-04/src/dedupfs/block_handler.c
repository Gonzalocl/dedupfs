#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "block_handler.h"

#define CONF_FILENAME "conf"



struct block_handler_conf* handler_conf = NULL;
char fs_path[PATH_MAX];

// pass an allocated struct
int block_handler_init(struct block_handler_conf *conf) {

    int err = 0;

    if ((handler_conf = malloc(sizeof(struct block_handler_conf))) == NULL) {
        // TODO man malloc no errno?
        return -errno;
    }

    // set default values
    if (conf == NULL) {

        if (access()) {

        }
        else {
            // TODO create conf file
//            handler_conf->blocks_path = DEFAULT_BLOCKS_PATH;
            handler_conf->block_size = DEFAULT_BLOCK_SIZE;
            handler_conf->hash_type = DEFAULT_HASH_TYPE;
            handler_conf->hash_length = DEFAULT_HASH_LENGTH;
            handler_conf->hash_split = DEFAULT_HASH_SPLIT;
            handler_conf->hash_split_size = DEFAULT_HASH_SPLIT_SIZE;
            handler_conf->bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;
        }
    }
    else {

        // TODO access common factor?
        if (access()) {

        }
        else {

        }

    }


    return err;
}

int block_handler_get_conf(struct block_handler_conf *conf) {
    // TODO make copy?
    return 0;
}

// Free blocks_path
int block_handler_end() {
    // TODO
    return 0;
}

int block_create(char *hash, char *data) {
    if (handler_conf == NULL) {
        // TODO
        // TODO call init?
        return -1;
    }
    if (hash == NULL) {
        // TODO
        return -2;
    }
    if (data == NULL) {
        // TODO
        return -3;
    }

    // TODO
    char block_path[4096];
    sprintf(block_path, "%s/%02x/%02x", handler_conf->blocks_path, hash[0], hash[1]);
    if (access(block_path, F_OK) != -1) {
        // file exists increase link counter
        int link_counter;
        FILE *block;
        block = fopen(block_path, "rb");
        if (block == NULL) {
            return -4;
        }

        // TODO is it more efficient with seek end?
        fseek(block, handler_conf->block_size, SEEK_SET);
        fread(&link_counter, handler_conf->bytes_link_counter, 1, block);
        link_counter++;
        fseek(block, handler_conf->block_size, SEEK_SET);
        fwrite(&link_counter, handler_conf->bytes_link_counter, 1 ,block);
        fclose(block);

    }
    else {
        // file doesn't exist create with link counter to 1

    }


    return 0;
}

int block_delete(char *hash) {
    if (handler_conf == NULL) {
        // TODO
        // TODO call init?
        return -1;
    }
    if (hash == NULL) {
        // TODO
        return -2;
    }

    return 0;
}


int block_read(char *hash, char *data) {
    if (handler_conf == NULL) {
        // TODO
        // TODO call init?
        return -1;
    }
    if (hash == NULL) {
        // TODO
        return -2;
    }
    if (data == NULL) {
        // TODO
        return -3;
    }

    return 0;
}

