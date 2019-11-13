#include <unistd.h>
#include <stdio.h>

#include "../file_handler.h"


int main () {
    int ret;
    ret = file_handler_init(NULL);
//    printf("init: %d\n", ret);
//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}



