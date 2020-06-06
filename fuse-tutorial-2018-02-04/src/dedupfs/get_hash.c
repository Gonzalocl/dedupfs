#include "get_hash.h"
#include <openssl/evp.h>

int get_hash(int hash_type, int data_size, const char *data, unsigned char *hash) {
    EVP_MD_CTX *ctx;
    const EVP_MD *md;
    int hash_len;

    if (hash_type == 1) {
        md = EVP_md5();
    }
    else if (hash_type == 2) {
        md = EVP_sha1();
    }
    else {
        return -1;
    }

    ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, data, data_size);
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);

}
