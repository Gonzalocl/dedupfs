#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>


int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("ERROR\n");
        return EXIT_FAILURE;
    }

    EVP_MD_CTX *ctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    int hash_len;

    if (strncmp(argv[1], "sha", 3) == 0) {
        md = EVP_sha1();
    }
    else if (strncmp(argv[1], "md5", 3) == 0) {
        md = EVP_md5();
    }
    else {
        printf("ERROR\n");
        return EXIT_FAILURE;
    }

    ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, argv[2], strlen(argv[2]));
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);

    printf("Hash len: %d\n", hash_len);
    printf("Hash: ");
    for (int i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");


    return EXIT_SUCCESS;

}



