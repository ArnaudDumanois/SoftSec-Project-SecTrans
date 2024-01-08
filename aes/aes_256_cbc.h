#ifndef SOFTSEC_PROJECT_SECTRANS_AES_256_CBC_H
#define SOFTSEC_PROJECT_SECTRANS_AES_256_CBC_H

#include <openssl/evp.h>
#include <openssl/aes.h>

void aes_init_encryption_ctx(EVP_CIPHER_CTX **e_ctx, unsigned char key[32], unsigned char iv[16]);

void aes_init_decryption_ctx(EVP_CIPHER_CTX **d_ctx, unsigned char key[32], unsigned char iv[16]);

int
aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, unsigned char key[32], unsigned char iv[16]);

unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);

#endif //SOFTSEC_PROJECT_SECTRANS_AES_256_CBC_H
