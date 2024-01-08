#include "rsa_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/err.h>

int get_max_number_of_bytes_auth(RSA *key) {
    return get_max_number_of_bytes_authorized(key);
}

int get_RSA_size(RSA *key) {
    return get_rsa_size(key);
}

RSA *generate_RSA_keys() {
    RSA *keys = generate_rsa_keys();
    if (keys == NULL) {
        fprintf(stderr, "Error generating RSA key pair\n");
        RSA_free(keys);
        return NULL;
    }
    return keys;
}

char *get_public_KEY_in_string(RSA *keys) {
    char *publicKeyStr = get_public_key_in_string(keys);
    if (publicKeyStr == NULL) {
        fprintf(stderr, "Error getting public key string\n");
        return NULL;
    }
    return publicKeyStr;
}

RSA *read_public_KEY_from_string(char *public_key) {
    RSA *readPublicKey = read_public_key_from_string(public_key);
    if (readPublicKey == NULL) {
        fprintf(stderr, "Error reading public key from string\n");
        RSA_free(readPublicKey);
        return NULL;
    }
    return readPublicKey;
}

int encrypt_message(char *msg_to_encrypt, RSA *public_key, char **encrypted_msg) {
    if (public_key == NULL) return -1;
    int encrypt_result = encrypt(msg_to_encrypt, public_key, encrypted_msg);
    if (encrypt_result == -1) {
        fprintf(stderr, "Error encrypting message\n");
        RSA_free(public_key);
        ERR_print_errors_fp(stderr);
        //free(*encrypted_msg);
        return -1;
    }
    return encrypt_result;
}

int decrypt_message(char *encrypted_msg, RSA *keys, char **plain_text_msg) {
    int decryptResult = decrypt(encrypted_msg, keys, plain_text_msg);
    if (decryptResult == -1) {
        fprintf(stderr, "Error decrypting message\n");
        return -1;
    }
    return 0;
}