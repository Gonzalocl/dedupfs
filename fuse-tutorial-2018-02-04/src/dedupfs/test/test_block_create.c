#include <unistd.h>
#include <stdio.h>

#include "../block_handler.h"

unsigned char hash[16] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0};


int main () {
    int ret;
    ret = block_handler_init("bin/fs", NULL);
//    printf("init: %d\n", ret);
    ret = block_create(hash, NULL);
//    printf("create: %d\n", ret);
//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}



