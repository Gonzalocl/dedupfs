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
    unsigned char *hash_read = malloc(cache->hash_length);
    unsigned char *hash_write = malloc(cache->hash_length);
    char *data = malloc(cache->block_size);
    long bytes_write = 0;

    if ((offset % cache->block_size) != 0) {
        block = offset/cache->block_size;
        long block_offset = offset % cache->block_size;

        // read blocks
        file_get_block_hash(cache->file_handler, cache->fd, block, hash_read);
        block_read(hash_read, data);

        // modify blocks
        bytes_write = size < (cache->block_size-block_offset) ? size : (cache->block_size-block_offset);
        memcpy(data+block_offset, buf, bytes_write);

        // create new blocks
        get_hash(cache->hash_type, cache->block_size, data, hash_write);
        block_create(hash_write, data);

        // set new blocks hashes
        file_set_block_hash(cache->file_handler, cache->fd, block, hash_write);

        // delete blocks
        block_delete(hash_read);
    }


    while ((size - bytes_write) >= cache->block_size) {
        block++;

        // create new blocks
        get_hash(cache->hash_type, cache->block_size, buf+bytes_write, hash_write);
        block_create(hash_write, buf+bytes_write);
        bytes_write+=cache->block_size;

        // read blocks
        file_get_block_hash(cache->file_handler, cache->fd, block, hash_read);

        // set new blocks hashes
        file_set_block_hash(cache->file_handler, cache->fd, block, hash_write);

        // delete blocks
        block_delete(hash_read);
    }

    long remaining_bytes = size - bytes_write;
    if ((remaining_bytes) > 0) {
        block++;

        // read blocks
        file_get_block_hash(cache->file_handler, cache->fd, block, hash_read);
        block_read(hash_read, data);

        // modify blocks
        memcpy(data, buf+bytes_write, remaining_bytes);

        // create new blocks
        get_hash(cache->hash_type, cache->block_size, data, hash_write);
        block_create(hash_write, data);

        // set new blocks hashes
        file_set_block_hash(cache->file_handler, cache->fd, block, hash_write);

        // delete blocks
        block_delete(hash_read);
    }


    free(hash_read);
    free(hash_write);
    free(data);
}

int cache_read(struct block_cache *cache, void *buf, size_t size, off_t offset) {

}

int cache_end(struct block_cache *cache) {
    return 0;
}

