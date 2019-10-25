#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../block_handler.h"

#define CONF_FILENAME "conf"
struct block_handler_conf* handler_conf = NULL;

int read_conf_file(char *blocks_path) {

    // TODO use open read write?
    // TODO max
    char path[4096];
    sprintf(path, "%s/%s", blocks_path, CONF_FILENAME);
    printf("path: %s\n", path);

    FILE * conf_file;
    if ((conf_file = fopen(path, "rb")) == NULL) {
        return -1;
    }

    int len;
    fread(&len, 1, 1, conf_file);
    fread(handler_conf->blocks_path, 1, len, conf_file);
    fread(&handler_conf->block_size, 4, 1, conf_file);
    fread(&handler_conf->hash_type, 4, 1, conf_file);
    fread(&handler_conf->hash_length, 4, 1, conf_file);
    fread(&handler_conf->hash_split, 4, 1, conf_file);
    fread(&handler_conf->bytes_link_counter, 4, 1, conf_file);


    fclose(conf_file);

    // TODO malloc str? set max
    // TODO dont use scanf
//    scanf("%s\n%d\n%d\n%d\n%d\n%d\n",
//            handler_conf->blocks_path,
//            &handler_conf->block_size,
//            &handler_conf->hash_type,
//            &handler_conf->hash_length,
//            &handler_conf->hash_split,
//            &handler_conf->bytes_link_counter);
}


int main () {

    handler_conf = malloc(sizeof(struct block_handler_conf));
    handler_conf->blocks_path = malloc(4096);
    read_conf_file("bin");

    printf("\n");
    printf("blocks_path:        %s\n", handler_conf->blocks_path);
    printf("block_size:         %d\n", handler_conf->block_size);
    printf("hash_type:          %d\n", handler_conf->hash_type);
    printf("hash_length:        %d\n", handler_conf->hash_length);
    printf("hash_split:         %d\n", handler_conf->hash_split);
    printf("bytes_link_counter: %d\n", handler_conf->bytes_link_counter);



    return 0;
}


