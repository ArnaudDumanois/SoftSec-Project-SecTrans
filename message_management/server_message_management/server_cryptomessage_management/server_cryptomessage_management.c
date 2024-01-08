#include "../../common_message_management.h"
#include "../../../actions.h"
#include "../../../base64/base64decode.h"
#include "../../client_message_management/client_message_management.h"
#include "../../../utils/array_utils.h"
#include <stdio.h>
#include <string.h>
#include "../../../aes/aes_manager.h"
#include "../../../rsa/rsa_manager.h"
#include "../../../base64/base64encode.h"

unsigned char *decode_msg(char msg[INPUT_SIZE]) {
    size_t decoded_size = b64_decoded_size(msg);
    unsigned char *out = malloc(decoded_size);
    if (b64_decode(msg, out, decoded_size) == -1) {
        printf("Error in the base64 decryption\n");
    }
    return out;
}

char *get_public_key_from_message(char *msg) {
    char *ptr2 = strchr(msg, ';'); // to get the first ;
    char *tok = strtok(ptr2 + 1, "\0");
    return tok;
}

unsigned char *read_rsa_public_key(char msg[INPUT_SIZE]) {
    char *plaintext_msg = (char *) decode_msg(msg);
    unsigned char *public_key = (unsigned char *) get_public_key_from_message(plaintext_msg);
    return public_key;
    //free(plaintext_msg); // TODO : move this
}

int get_key_and_iv(unsigned char key[32], unsigned char iv[16]) {
    EVP_CIPHER_CTX *e_ctx;
    if (init_aes(key, iv) != 0) {
        printf("error init aes\n");
        return -1;
    }
    init_aes_encryption_ctx(&e_ctx, key, iv);

    printf("key : %s\n", key);
    printf("key size 2: %d\n", EVP_CIPHER_CTX_key_length(e_ctx));

    printf("iv : %s\n", EVP_CIPHER_CTX_original_iv(e_ctx));
    printf("iv size : %d\n", EVP_CIPHER_CTX_iv_length(e_ctx));

    if (EVP_CIPHER_CTX_key_length(e_ctx) != 32 || EVP_CIPHER_CTX_iv_length(e_ctx) != 16) return -1;
    return 0;
}

int send_aes_ciphered_rsa_public_key(unsigned char *public_key, char msg_to_send[INPUT_SIZE]) {
    clear_array(msg_to_send, INPUT_SIZE);

    unsigned char key[32];
    unsigned char iv[16];

    if (get_key_and_iv(key, iv) == -1) {
        return -1;
    }

    add_action(msg_to_send, ACTION_AES_SEND);
    add_string(msg_to_send, (char *) key);
    add_string_without_dot(msg_to_send, (char *) iv);


    RSA *rsa = read_public_key_from_string((char *) public_key);
    if (rsa == NULL) return -1;

    int public_key_size = get_RSA_size(rsa);
    int msg_size = (public_key_size * 2) + 3;
    int max_auth = get_max_number_of_bytes_auth(rsa);
    size_t encoded_size = b64_encoded_size(msg_size);
    if (msg_size > max_auth || encoded_size >= INPUT_SIZE) return -1;

    char *encrypted_msg;
    int res = encrypt_message(msg_to_send, rsa, &encrypted_msg);
    if (res == -1) return -1;

    char *encoded = b64_encode((unsigned char *) msg_to_send, msg_size);
    sending(encoded, CLIENT_PORT);
    clear_array(msg_to_send, INPUT_SIZE);

    RSA_free(rsa);
    free(public_key);
    free(encoded);
    free(encrypted_msg);
    clear_array(msg_to_send, INPUT_SIZE);

    return 0;
}