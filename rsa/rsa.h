#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_H

#include <openssl/ssl.h>
#include <openssl/rsa.h>

RSA *generateRSAKeyPair();


int encryptMessage(const char *plainText, RSA *publicKey, char **encryptedText);

int decryptMessage(const char *encryptedText, RSA *privateKey, char **decryptedText);
RSA *readPublicKeyFromStr(const char *keyStr);
char *getPublicKeyStr(RSA *rsa);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_H
