#include <stdio.h>
#include <string.h>
#include "get_hash.h"


int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("ERROR\n");
        return -1;
    }

    int hash_type;
    int hash_len = 1024;
    unsigned char hash[hash_len];

    if (strncmp(argv[1], "sha", 3) == 0) {
        hash_type = 2;
    }
    else if (strncmp(argv[1], "md5", 3) == 0) {
        hash_type = 1;
    }
    else {
        printf("ERROR\n");
        return -1;
    }

    get_hash(hash_type, strlen(argv[2]), argv[2], hash);

    printf("Hash len: %d\n", hash_len);
    printf("Hash: ");
    for (int i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");


    return 0;

}



