#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aes_256_cbc.h"

#define KEY_DATA "ROUDIER"

int init_aes(EVP_CIPHER_CTX **en, EVP_CIPHER_CTX **de) {
    /* "opaque" encryption, decryption ctx structures that libcrypto uses to record
        status of enc/dec operations */
    *en = EVP_CIPHER_CTX_new();
    *de = EVP_CIPHER_CTX_new();
    unsigned int salt[] = {12345, 54321};
    unsigned char *key_data;
    int key_data_len;

    key_data = (unsigned char *) KEY_DATA;
    key_data_len = strlen(KEY_DATA);

    if (aes_init(key_data, key_data_len, (unsigned char *) &salt, *en, *de)) {
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

void decrypt_aes(unsigned char *encrypted_msg, char **decrypted_msg, int len, int olen, EVP_CIPHER_CTX *de) {
    *decrypted_msg = (char *) aes_decrypt(de, encrypted_msg, &len);

    /*free(ciphertext); // TODO : move the free
    free(plaintext);
    EVP_CIPHER_CTX_free(en);
    EVP_CIPHER_CTX_free(de);*/
}