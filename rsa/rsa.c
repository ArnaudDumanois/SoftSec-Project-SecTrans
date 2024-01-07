#include "rsa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/err.h>

#define KEY_LENGTH  2048
#define PUB_EXP     65537

RSA *generate_rsa_keys() {
    RSA *rsa = NULL;
    BIGNUM *bne = NULL;

    int bits = KEY_LENGTH; // Adjust the key size as needed

    bne = BN_new();
    BN_set_word(bne, PUB_EXP);

    rsa = RSA_new();
    RSA_generate_key_ex(rsa, bits, bne, NULL);

    BN_free(bne);
    return rsa;
}

int encrypt(const char *plainText, RSA *publicKey, char **encryptedText) {
    int rsaSize = RSA_size(publicKey);
    printf("size public : %d\n", rsaSize);
    *encryptedText = (char *) malloc(rsaSize);

    int result = RSA_public_encrypt(strlen(plainText), (const unsigned char *) plainText,
                                    (unsigned char *) *encryptedText, publicKey, RSA_PKCS1_OAEP_PADDING);

    return result;
}

int decrypt(const char *encryptedText, RSA *privateKey, char **decryptedText) {
    int rsaSize = RSA_size(privateKey);
    printf("size private : %d\n", rsaSize);
    *decryptedText = (char *) malloc(rsaSize);

    int result = RSA_private_decrypt(rsaSize, (const unsigned char *) encryptedText, (unsigned char *) *decryptedText,
                                     privateKey, RSA_PKCS1_OAEP_PADDING);

    return result;
}

RSA *read_public_key_from_string(const char *keyStr) {
    BIO *bio = BIO_new_mem_buf(keyStr, -1);
    if (bio == NULL) {
        perror("Error creating BIO");
        return NULL;
    }

    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (rsa == NULL) {
        perror("Error reading public key from string");
        BIO_free(bio);
        return NULL;
    }

    BIO_free(bio);
    return rsa;
}

char *get_public_key_in_string(RSA *rsa) {
    BIO *bio = BIO_new(BIO_s_mem());
    if (bio == NULL) {
        perror("Error creating BIO");
        return NULL;
    }

    if (PEM_write_bio_RSA_PUBKEY(bio, rsa) != 1) {
        perror("Error writing public key to BIO");
        BIO_free(bio);
        return NULL;
    }

    char *buffer;
    long length = BIO_get_mem_data(bio, &buffer);

    char *result = malloc(length + 1);
    if (result == NULL) {
        perror("Error allocating memory for public key string");
        BIO_free(bio);
        return NULL;
    }

    memcpy(result, buffer, length);
    result[length] = '\0';

    BIO_free(bio);
    return result;
}

