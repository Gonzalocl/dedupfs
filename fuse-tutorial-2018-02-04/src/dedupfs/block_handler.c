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

#define BLOCK_CONF "block_conf"

#define HASHES_SUPPORTED 2
int hash_length[HASHES_SUPPORTED] = {16, 20};


struct block_handler_conf *handler_conf = NULL;
char fs_path[PATH_MAX];

static int check_conf(struct block_handler_conf *conf) {
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
static int write_conf_file() {

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

    if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, handler_conf->blocks_path, BLOCK_CONF) < 0) {
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
static int read_conf_file(char *blocks_path) {

    // TODO use open read write?
    char path[PATH_MAX];
    int path_length;
    int ret_value = 0;


    if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, blocks_path, BLOCK_CONF) < 0) {
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
        ret_value = -errno;
        goto error0;
    }


    if (conf == NULL) {

        if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, DEFAULT_BLOCKS_PATH, BLOCK_CONF) < 0) {
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

        if (snprintf(path, PATH_MAX, "%s/%s/%s", fs_path, conf->blocks_path, BLOCK_CONF) < 0) {
            ret_value = -EIO;
            goto error1;
        }

        if (access(path, F_OK) == 0) {
            if ((ret_value = read_conf_file(conf->blocks_path)) < 0) {
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

    memcpy(conf, handler_conf, sizeof(struct block_handler_conf));
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

// TODO return -errno (-EIO)
static int get_block_path(char *block_path, unsigned char *hash) {
    int chars;
    if ((chars = snprintf(block_path, PATH_MAX, "%s/%s/", fs_path, handler_conf->blocks_path)) < 0) {
        return -1;
    }
    // TODO make this check anywhere else only once
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

// TODO return -errno (-EIO)
static int create_parents(unsigned char *hash) {
    char block_path[PATH_MAX];
    int chars;
    if ((chars = snprintf(block_path, PATH_MAX, "%s/%s/", fs_path, handler_conf->blocks_path)) < 0) {
        return -1;
    }
    // TODO make this check anywhere else only once
    if ((chars + handler_conf->hash_length*2 + handler_conf->hash_split + 1) > PATH_MAX) {
        return -1;
    }
    // TODO optimize this?
    for (int i = 0; i < handler_conf->hash_split; i++) {
        for (int j = 0; j < handler_conf->hash_split_size; j++) {
            if (sprintf(block_path + chars, "%02x", hash[handler_conf->hash_split_size*i + j]) < 0) {
                // TODO goto error undo mkdir done
                return -1;
            }
            chars += 2;
        }
        if (mkdir(block_path, 0755) != 0) {
            if (errno != EEXIST) {
                // TODO goto error undo mkdir done
                return -errno;
            }
        }
        else {
            // TODO take note folder created in case it has to be undone
        }
        block_path[chars] = '/';
        chars++;
    }
    return 0;
}

static int delete_block_path(char *block_path) {
    // TODO
    return 0;
}

int block_create(unsigned char *hash, const char *data) {
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
    
    if ((ret_value = get_block_path(block_path, hash)) != 0) {
        return ret_value;
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
        // TODO undo this if error with delete path
        if ((ret_value = create_parents(hash)) != 0) {
            return ret_value;
        }

        if ((block = fopen(block_path, "w")) == NULL) {
            return -errno;
        }

        if (fwrite(data, 1, handler_conf->block_size, block) != handler_conf->block_size) {
            ret_value = -EIO;
            goto out;
        }

        link_counter = 1;

        if (fwrite(&link_counter, handler_conf->bytes_link_counter, 1, block) != 1) {
            ret_value = -EIO;
            goto out;
        }

    }


    out:
    if (fclose(block) != 0) {
        return -errno;
    }
    return ret_value;
}

int block_delete(unsigned char *hash) {
//    if (handler_conf == NULL) {
//         TODO
//         TODO call init?
//        return -1;
//    }
//    if (hash == NULL) {
//         TODO
//        return -2;
//    }
    int ret_value = 0;
    FILE *block;
    int link_counter;
    char block_path[PATH_MAX];

    if ((ret_value = get_block_path(block_path, hash)) != 0) {
        return ret_value;
    }
    if ((block = fopen(block_path, "r+b")) == NULL) {
        return -errno;
    }
    // TODO is it more efficient with seek end?
    if (fseek(block, handler_conf->block_size, SEEK_SET) != 0) {
        ret_value = -errno;
        goto error;
    }

    if (fread(&link_counter, handler_conf->bytes_link_counter, 1, block) != 1) {
        ret_value = -EIO;
        goto error;
    }

    if (link_counter == 1) {
        // delete block and path
        if (fclose(block) != 0) {
            return -errno;
        }

        if (unlink(block_path) != 0) {
            // TODO no undo?
            return -errno;
        }

        if ((ret_value = delete_block_path(block_path)) != 0) {
            return ret_value;
        }

    }
    else {
        // decrease link_counter
        link_counter--;

        if (fseek(block, handler_conf->block_size, SEEK_SET) != 0) {
            ret_value = -errno;
            goto error;
        }

        if (fwrite(&link_counter, handler_conf->bytes_link_counter, 1, block) != 1) {
            ret_value = -EIO;
            goto error;
        }

        if (fclose(block) != 0) {
            return -errno;
        }

    }

    return 0;
    error:
    if (fclose(block) != 0) {
        return -errno;
    }
    return ret_value;
}


int block_read(unsigned char *hash, char *data) {
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
    char block_path[PATH_MAX];

    if ((ret_value = get_block_path(block_path, hash)) != 0) {
        return ret_value;
    }

    if ((block = fopen(block_path, "rb")) == NULL) {
        return -errno;
    }

    if (fread(data, 1, handler_conf->block_size, block) != handler_conf->block_size) {
        // no goto because it is the same
        ret_value = -EIO;
    }

    if (fclose(block) != 0) {
        return -errno;
    }

    return ret_value;
}

