#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../get_hash.h"

#define HASHES_SUPPORTED 2
static int hash_length[HASHES_SUPPORTED] = {16, 20};

int main (int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "ERROR:%s:%d param\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    int hash_type = atoi(argv[1]);
    int data_size = atoi(argv[2]);
    char data[data_size];
    unsigned char hash[hash_length[hash_type-1]];

    for (int i = 0; i < data_size; i++) {
        sscanf(argv[3] + i*2, "%02hhx", &data[i]);
    }

    int ret;
    if ((ret = get_hash(hash_type, data_size, data, hash)) < 0) {
        fprintf(stderr, "ERROR:%s:%d get_hash: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < hash_length[hash_type-1]; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

//    printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return EXIT_SUCCESS;
}


