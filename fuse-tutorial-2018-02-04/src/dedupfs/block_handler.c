#include "block_handler.h"


struct block_handler_conf* handler_conf = NULL;


// pass an allocated struct
int block_handler_init(struct block_handler_conf *conf) {
    // TODO this only sets default values
    // set default values
    if (conf == NULL) {
        handler_conf = malloc(sizeof(struct block_handler_conf));
        handler_conf->blocks_path = "blocks";
        handler_conf->block_size = 32;
        handler_conf->hash_type = 0;
        handler_conf->hash_split = 0;
        handler_conf->bytes_link_counter = 4;
    }
    return 0;
}
int block_handler_get_conf(struct block_handler_conf *conf) {
    // TODO
    return 0;
}

// Free blocks_path
int block_handler_end() {
    // TODO
    return 0;
}

int block_create(char *hash, char *data) {
    if (conf == NULL) {
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
    if (conf == NULL) {
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
    if (conf == NULL) {
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

