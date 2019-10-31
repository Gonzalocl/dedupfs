#ifndef BLOCK_HANDLER_H
#define BLOCK_HANDLER_H

#include <limits.h>

#define DEFAULT_BLOCKS_PATH "blocks"
#define DEFAULT_BLOCK_SIZE 32
// TODO
#define DEFAULT_HASH_TYPE 1
#define DEFAULT_HASH_LENGTH 16
#define DEFAULT_HASH_SPLIT 2
#define DEFAULT_HASH_SPLIT_SIZE 1
#define DEFAULT_BYTES_LINK_COUNTER 4

struct block_handler_conf {
    char blocks_path[PATH_MAX];
    int block_size;
    int hash_type;
    int hash_length;
    int hash_split;
    int hash_split_size;
    int bytes_link_counter;
};


// pass an allocated struct
int block_handler_init(char *fs, struct block_handler_conf *conf);
int block_handler_get_conf(struct block_handler_conf *conf);

// Free blocks_path
int block_handler_end();


int block_create(char *hash, char *data);
int block_delete(char *hash);


int block_read(char *hash, char *data);

// Doesn't make sense
//int block_write(char *hash, char *data);

// Could be useful
//int block_read_delete(char *hash, char *data);














#endif //BLOCK_HANDLER_H
