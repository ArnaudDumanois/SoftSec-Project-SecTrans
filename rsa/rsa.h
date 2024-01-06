#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_H

#include <openssl/ssl.h>
#include <openssl/rsa.h>

void generate_keys(char **private_key, char **public_key);

int encrypt(const unsigned char *plaintext, int plaintextLen, RSA *publicKey, unsigned char *ciphertext);

int decrypt(const unsigned char *ciphertext, int ciphertextLen, RSA *privateKey, unsigned char *plaintext);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_H
