#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "block_handler.h"

#define CONF_FILENAME "conf"



struct block_handler_conf* handler_conf = NULL;
char fs_path[PATH_MAX];

/*
 * create configuration file and write it
 * the folder should exist handler_conf allocated and
 * fs_path set
 */
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

/*
 * read configuration file
 * file should exist handler_conf allocated and
 * fs_path set
 */
int read_conf_file(char *blocks_path) {

    // TODO use open read write?
    char path[PATH_MAX];
    int path_length;
    int ret_value = 0;


    if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, blocks_path, CONF_FILENAME) < 0) {
        return -EIO;
    }

    FILE * conf_file;
    if ((conf_file = fopen(path, "rb")) == NULL) {
        return -errno;
    }

    // TODO check this can be negative
    if (fread(&path_length, 4, 1, conf_file) != 1) {
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
    if (fread(&handler_conf->hash_split_size, 4, 1, conf_file) != 1) {
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

// pass an allocated struct
int block_handler_init(char *fs, struct block_handler_conf *conf) {

    int ret_value = 0;

    if (handler_conf != NULL) {
        // TODO
        return -1;
    }

    strncpy(fs_path, fs, PATH_MAX-1);
    // TODO
//    if (PATH_MAX > 0) {
        fs_path[PATH_MAX-1] = '\0';
//    }

    if ((handler_conf = malloc(sizeof(struct block_handler_conf))) == NULL) {
        // TODO man malloc no errno?
        return -errno;
    }


    if (conf == NULL) {

        // TODO check folder exists
        if (access(DEFAULT_BLOCKS_PATH)) {
            read_conf_file(DEFAULT_BLOCKS_PATH);
        }
        else {
            strncpy(handler_conf->blocks_path, DEFAULT_BLOCKS_PATH);
            handler_conf->block_size = DEFAULT_BLOCK_SIZE;
            handler_conf->hash_type = DEFAULT_HASH_TYPE;
            handler_conf->hash_length = DEFAULT_HASH_LENGTH;
            handler_conf->hash_split = DEFAULT_HASH_SPLIT;
            handler_conf->hash_split_size = DEFAULT_HASH_SPLIT_SIZE;
            handler_conf->bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;
            write_conf_file();
        }
    }
    else {

        // TODO access common factor?
        // TODO check folder exists
        if (access(conf->blocks_path)) {
            read_conf_file(conf->blocks_path);
            // TODO check no defautl parameters are ok check other parameters are ok
        }
        else {
            // TODO copy parameter and check values are ok and write

        }

    }

    return 0;
    error:
    free(handler_conf);
    fs_path[0] = '\0';
    return ret_value;
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
//    sprintf(block_path, "%s/%02x/%02x", handler_conf->blocks_path, hash[0], hash[1]);
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

