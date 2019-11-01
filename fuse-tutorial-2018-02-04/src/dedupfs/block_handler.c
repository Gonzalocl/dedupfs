#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "block_handler.h"

#define TRUE (0==0)
#define FALSE (!TRUE)

#define CONF_FILENAME "conf"

#define HASHES_SUPPORTED 2
int hash_length[HASHES_SUPPORTED] = {16, 20};


struct block_handler_conf* handler_conf = NULL;
char fs_path[PATH_MAX];

int check_conf(struct block_handler_conf* conf) {
    // TODO complete
    if (conf->block_size <= 0) {
        return FALSE;
    }

    if (conf->hash_type <= 0 || conf->hash_type > HASHES_SUPPORTED) {
        return FALSE;
    }
    if (conf->hash_length != hash_length[conf->hash_type-1]) {
        return FALSE;
    }

    if (conf->hash_split < 0) {
        return FALSE;
    }
    if (conf->hash_split_size <= 0) {
        return FALSE;
    }
    if ( (conf->hash_split*conf->hash_split_size) >= conf->hash_length) {
        return FALSE;
    }

    if (conf->bytes_link_counter <= 0) {
        return FALSE;
    }

    return TRUE;
}

/*
 * create configuration file and write it
 * handler_conf should be allocated and
 * fs_path set
 */
int write_conf_file() {

    // TODO use open read write?
    // TODO delete if error
    char path[PATH_MAX];
    int path_length;
    int ret_value = 0;
    int folder_created = FALSE;

    if (snprintf(path, PATH_MAX, "%s/%s", fs_path, handler_conf->blocks_path) < 0) {
        return -EIO;
    }
    if (mkdir(path, 0755) != 0) {
        if (errno != EEXIST) {
            return -errno;
        }
    }
    else {
        folder_created = TRUE;
    }

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

    if (fread(&path_length, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
    if (path_length < 0 || path_length >= PATH_MAX) {
        ret_value = -EIO;
        goto out;
    }
    if (fread(handler_conf->blocks_path, 1, path_length, conf_file) != path_length) {
        ret_value = -EIO;
        goto out;
    }
    handler_conf->blocks_path[path_length] = '\0';

    if (fread(&handler_conf->block_size, 4, 1, conf_file) != 1) {
        ret_value = -EIO;
        goto out;
    }
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

    if (!check_conf(handler_conf)) {
        ret_value = -ENOTRECOVERABLE;
    }

    out:
    if (fclose(conf_file) != 0) {
        return -errno;
    }
    return ret_value;
}

// pass an allocated struct
int block_handler_init(char *fs, struct block_handler_conf *conf) {

    char path[PATH_MAX];
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
        ret_value = -1;
        goto error0;
    }


    if (conf == NULL) {

        if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, DEFAULT_BLOCKS_PATH, CONF_FILENAME) < 0) {
            ret_value = -EIO;
            goto error1;
        }
        // TODO how to determine if other error in access
        if (access(path, F_OK) == 0) {
            if ((ret_value = read_conf_file(DEFAULT_BLOCKS_PATH)) < 0) {
                goto error1;
            }
        }
        else {
            strncpy(handler_conf->blocks_path, DEFAULT_BLOCKS_PATH, PATH_MAX);
            handler_conf->block_size = DEFAULT_BLOCK_SIZE;
            handler_conf->hash_type = DEFAULT_HASH_TYPE;
            handler_conf->hash_length = DEFAULT_HASH_LENGTH;
            handler_conf->hash_split = DEFAULT_HASH_SPLIT;
            handler_conf->hash_split_size = DEFAULT_HASH_SPLIT_SIZE;
            handler_conf->bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;
            if ((ret_value = write_conf_file()) < 0) {
                goto error1;
            }
        }
    }
    else {

        if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, conf->blocks_path, CONF_FILENAME) < 0) {
            ret_value = -EIO;
            goto error1;
        }

        if (access(conf->blocks_path, F_OK)) {
            if ((ret_value = read_conf_file(DEFAULT_BLOCKS_PATH)) < 0) {
                goto error1;
            }
            if (conf->blocks_path[0] != 0 && strncmp(conf->blocks_path, handler_conf->blocks_path, PATH_MAX) != 0) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->block_size != 0 && conf->block_size != handler_conf->block_size) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->hash_type != 0 && conf->hash_type != handler_conf->hash_type) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->hash_length != 0 && conf->hash_length != handler_conf->hash_length) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->hash_split != 0 && conf->hash_split != handler_conf->hash_split) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->hash_split_size != 0 && conf->hash_split_size != handler_conf->hash_split_size) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }
            if (conf->bytes_link_counter != 0 && conf->bytes_link_counter != handler_conf->bytes_link_counter) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }

        }
        else {
            memcpy(handler_conf, conf, sizeof(struct block_handler_conf));

            if (handler_conf->blocks_path[0] == 0) strncpy(handler_conf->blocks_path, DEFAULT_BLOCKS_PATH, PATH_MAX);
            if (handler_conf->block_size == 0) handler_conf->block_size = DEFAULT_BLOCK_SIZE;
            if (handler_conf->hash_type == 0) handler_conf->hash_type = DEFAULT_HASH_TYPE;
            if (handler_conf->hash_length == 0) handler_conf->hash_length = DEFAULT_HASH_LENGTH;
            if (handler_conf->hash_split == 0) handler_conf->hash_split = DEFAULT_HASH_SPLIT;
            if (handler_conf->hash_split_size == 0) handler_conf->hash_split_size = DEFAULT_HASH_SPLIT_SIZE;
            if (handler_conf->bytes_link_counter == 0) handler_conf->bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;

            if (!check_conf(handler_conf)) {
                ret_value = -ENOTRECOVERABLE;
                goto error1;
            }

            if ((ret_value = write_conf_file()) < 0) {
                goto error1;
            }
        }

    }

    return 0;
    error1:
    free(handler_conf);
    error0:
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

int get_block_path(char *block_path, unsigned char *hash) {
    int chars;
    if ((chars = snprintf(block_path, PATH_MAX, "%s/%s/", fs_path, handler_conf->blocks_path)) < 0) {
        return -1;
    }
    if ((chars + handler_conf->hash_length*2 + handler_conf->hash_split + 1) > PATH_MAX) {
        return -1;
    }
    // TODO optimize this?
    for (int i = 0; i < handler_conf->hash_split; i++) {
        for (int j = 0; j < handler_conf->hash_split_size; j++) {
            if (sprintf(block_path + chars, "%02x", hash[handler_conf->hash_split_size*i + j]) < 0) {
                return -1;
            }
            chars += 2;
        }
        block_path[chars] = '/';
        chars++;
    }
    for (int i = handler_conf->hash_split*handler_conf->hash_split_size; i < handler_conf->hash_length; i++) {
        if (sprintf(block_path + chars, "%02x", hash[i]) < 0) {
            return -1;
        }
        chars += 2;
    }
    return 0;
}

int create_parents(unsigned char *hash) {
    // TODO cd + mkdir + ... or mkdir + mkdir + ...
    int chars;
    if ((chars = snprintf(block_path, PATH_MAX, "%s/%s/", fs_path, handler_conf->blocks_path)) < 0) {
        return -1;
    }
    if ((chars + handler_conf->hash_length*2 + handler_conf->hash_split + 1) > PATH_MAX) {
        return -1;
    }

}

int delete_path(unsigned)

int block_create(unsigned char *hash, char *data) {
//    if (handler_conf == NULL) {
//        // TODO
//        // TODO call init?
//        return -1;
//    }
//    if (hash == NULL) {
//        // TODO
//        return -2;
//    }
//    if (data == NULL) {
//        // TODO
//        return -3;
//    }

    int ret_value = 0;
    FILE *block;
    int link_counter;
    char block_path[PATH_MAX];
    
    if (get_block_path(block_path, hash) != 0) {
        return -EIO;
    }
    if (access(block_path, F_OK) == 0) {
        // file exists increase link counter
//        TODO
//        block = fopen(block_path, "rb");
        if ((block = fopen(block_path, "r+b")) == NULL) {
            return -errno;
        }

        // TODO is it more efficient with seek end?
        if (fseek(block, handler_conf->block_size, SEEK_SET) != 0) {
            ret_value = -errno;
            goto out;
        }

        if (fread(&link_counter, handler_conf->bytes_link_counter, 1, block) != 1) {
            ret_value = -EIO;
            goto out;
        }

        link_counter++;

        if (fseek(block, handler_conf->block_size, SEEK_SET) != 0) {
            ret_value = -errno;
            goto out;
        }

        if (fwrite(&link_counter, handler_conf->bytes_link_counter, 1, block) != 1) {
            ret_value = -EIO;
            goto out;
        }

    }
    else {
        // file doesn't exist create with link counter to 1

    }


    out:
    if (fclose(block) != 0) {
        return -errno;
    }
    return ret_value;
}

int block_delete(unsigned char *hash) {
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


int block_read(unsigned char *hash, char *data) {
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

