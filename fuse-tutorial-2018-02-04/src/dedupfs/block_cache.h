#ifndef BLOCK_CACHE_H
#define BLOCK_CACHE_H

#include "file_handler.h"

/*size in blocks*/
struct block_cache * cache_init(int fd, int size, struct file_handler_conf *file_handler);

int cache_write(struct block_cache *cache, long block, const void *buf);
int cache_read(struct block_cache *cache, long block, void *buf);

int cache_end(struct block_cache *cache);



#endif //BLOCK_CACHE_H
