#include "block_cache.h"


struct block_cache * cache_init(int fd, int size, struct file_handler_conf *file_handler) {
    file_read(file_handler, fd, 0, 0, 0);
}

int cache_write(struct block_cache *cache, long block, const void *buf);
int cache_read(struct block_cache *cache, long block, void *buf);

int cache_end(struct block_cache *cache) {
    return 0;
}

