#ifndef SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H
#define SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H

#include <openssl/evp.h>
#include <openssl/aes.h>

int init_aes(EVP_CIPHER_CTX **en, EVP_CIPHER_CTX **de);
void encrypt_aes(char *msg_to_encrypt, char **encrypted_msg, EVP_CIPHER_CTX *en);
void decrypt_aes(char *encrypted_msg, char **decrypted_msg, int len, int olen, EVP_CIPHER_CTX *de);

#endif //SOFTSEC_PROJECT_SECTRANS_AES_MANAGER_H
