#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aes_256_cbc.h"
#include "aes_manager.h"
#include <openssl/rand.h>

void generate_random_key_data(unsigned char *key, int key_len) {
    if (RAND_bytes(key, key_len) != 1) {
        fprintf(stderr, "Error generating random key.\n");
        exit(EXIT_FAILURE);
    }
}

void init_aes_encryption_ctx(EVP_CIPHER_CTX **e_ctx, unsigned char key[32], unsigned char iv[16]) {
    init_aes_encryption_ctx(e_ctx, key, iv);
}

void init_aes_decryption_ctx(EVP_CIPHER_CTX **d_ctx, unsigned char key[32], unsigned char iv[16]) {
    init_aes_decryption_ctx(d_ctx, key, iv);
}

int init_aes(unsigned char key[32], unsigned char iv[16]) {
    /* "opaque" encryption, decryption ctx structures that libcrypto uses to record
        status of enc/dec operations */

    unsigned int salt[] = {12345, 54321};
    unsigned char *key_data = malloc(sizeof(char) * 32);
    int key_data_len = 32;

    generate_random_key_data(key_data, key_data_len);

    if (aes_init(key_data, key_data_len, (unsigned char *) &salt, key, iv)) {
        printf("Couldn't initialize AES cipher\n");
        return -1;
    }
    return 0;
}

void encrypt_aes(char *msg_to_encrypt, unsigned char **encrypted_msg, EVP_CIPHER_CTX *en) {
    int len;
    /* The enc/dec functions deal with binary data and not C strings. strlen() will
       return length of the string without counting the '\0' string marker. We always
       pass in the marker byte to the encrypt/decrypt functions so that after decryption
       we end up with a legal C string */
    len = (int) strlen(msg_to_encrypt) + 1;
    *encrypted_msg = aes_encrypt(en, (unsigned char *) msg_to_encrypt, &len);
}

void decrypt_aes(unsigned char *encrypted_msg, char **decrypted_msg, int len, EVP_CIPHER_CTX *de) {
    *decrypted_msg = (char *) aes_decrypt(de, encrypted_msg, &len);
    /*free(ciphertext); // TODO : move the free
    free(plaintext);
    EVP_CIPHER_CTX_free(en);
    EVP_CIPHER_CTX_free(de);*/
}