#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>

char *get_ciphertext_from_plaintext(char *plaintext, int length_plaintext, RSA *public_key) {
    int public_key_size = RSA_size(public_key);
    char *ciphertext = malloc(public_key_size + 1);
    int encrypted_length = encrypt(plaintext, length_plaintext - 1, public_key, ciphertext);
    if (encrypted_length == -1) {
        printf("Encryption failed.\n");
        exit(EXIT_FAILURE);
    }
    ciphertext[public_key_size] = '\0';
    return ciphertext;
}

char *get_plaintext_from_ciphertext(char *ciphertext, int length_ciphertext, char *private_key) {
    int private_key_size = RSA_size(private_key);
    char *plaintext = malloc(private_key_size + 1);
    int decrypted_length = decrypt(plaintext, length_ciphertext - 1, private_key, ciphertext);
    if (decrypted_length == -1) {
        printf("Decryption failed.\n");
        exit(EXIT_FAILURE);
    }
    plaintext[private_key_size] = '\0';
    return plaintext;
}