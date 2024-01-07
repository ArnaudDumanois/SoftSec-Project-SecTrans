#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_H

#include <openssl/ssl.h>
#include <openssl/rsa.h>

RSA *generate_rsa_keys();

int encrypt(const char *plainText, RSA *publicKey, char **encryptedText);

int decrypt(const char *encryptedText, RSA *privateKey, char **decryptedText);

RSA *read_public_key_from_string(const char *keyStr);

char *get_public_key_in_string(RSA *rsa);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_H
