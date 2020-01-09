#include <stdlib.h>
#include <string.h>

#include "block_cache.h"
#include "block_handler.h"
#include "get_hash.h"

struct block_cache {
    struct file_handler_conf *file_handler;
    int fd;
    int size;
    int block_size;
    int hash_type;
    int hash_length;
};

struct block_cache * cache_init(int fd, int size, struct file_handler_conf *file_handler) {
    struct block_cache *cache = malloc(sizeof(struct block_cache));
    cache->fd = fd;
    cache->size = size;
    cache->file_handler = file_handler;
    cache->block_size = file_handler->block_handler.block_size;
    cache->hash_type = file_handler->block_handler.hash_type;
    cache->hash_length = file_handler->block_handler.hash_length;
}

int cache_write(struct block_cache *cache, const void *buf, size_t size, off_t offset) {

    long block;
    unsigned char *hash = malloc(cache->hash_length);
    char *data = malloc(cache->block_size);
    long bytes_write = 0;

    if ((offset % cache->block_size) != 0) {
        block = offset/cache->block_size;
        long block_offset = offset % cache->block_size;

        // read blocks
        file_get_block_hash(cache->file_handler, cache->fd, block, hash);
        block_read(hash, data);

        // modify blocks
        bytes_write = size < (cache->block_size-block_offset) ? size : (cache->block_size-block_offset);
        memcpy(data+block_offset, buf, bytes_write);

        // create new blocks
        get_hash(cache->hash_type, cache->hash_length, data, hash);
        block_create(hash, data);

        // set new blocks hashes
        file_set_block_hash(cache->file_handler, cache->fd, block, hash);

        // delete blocks
        block_delete(hash);
    }

//    for (int i = 0; i < ; i++) {
//
//    }
//
//    long last_write_byte = size+offset;
//    if ((last_write_byte % cache->block_size) != 0) {
//
//    }


    free(hash);
    free(data);
}

int cache_read(struct block_cache *cache, void *buf, size_t size, off_t offset) {

}

int cache_end(struct block_cache *cache) {
    return 0;
}

