#ifndef SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H
#define SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H

#include <openssl/evp.h>
#include <openssl/aes.h>

void init_aes_encryption_ctx(EVP_CIPHER_CTX **e_ctx, unsigned char key[32], unsigned char iv[16]);

void init_aes_decryption_ctx(EVP_CIPHER_CTX **d_ctx, unsigned char key[32], unsigned char iv[16]);
int init_aes(unsigned char key[32], unsigned char iv[16]);
void encrypt_aes(char *msg_to_encrypt, unsigned char **encrypted_msg, EVP_CIPHER_CTX *en);
void decrypt_aes(unsigned char *encrypted_msg, char **decrypted_msg, int len, EVP_CIPHER_CTX *de);

#endif //SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H
