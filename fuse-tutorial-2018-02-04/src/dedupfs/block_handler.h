#ifndef BLOCK_HANDLER_H
#define BLOCK_HANDLER_H

struct block_handler_conf {
    char *blocks_path;
    int block_size;
    int hash_type;
    int hash_length;
    int hash_split;
    int bytes_link_counter;
};


// pass an allocated struct
int block_handler_init(struct block_handler_conf *conf);
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
