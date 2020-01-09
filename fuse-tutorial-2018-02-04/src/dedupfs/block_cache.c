#include "block_cache.h"


struct block_cache * cache_init(int fd, int size, struct file_handler_conf *file_handler) {

}

int cache_write(struct block_cache *cache, const void *buf, size_t size, off_t offset) {
    // read blocks
    // modify blocks
    // create new blocks
    // set new blocks hashes
    // delete blocks
}

int cache_read(struct block_cache *cache, void *buf, size_t size, off_t offset) {

}

int cache_end(struct block_cache *cache) {
    return 0;
}

