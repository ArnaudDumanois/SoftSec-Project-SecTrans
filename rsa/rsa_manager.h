#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H

#include "rsa.h"

RSA *generate_RSA_keys();

int encrypt_message(char *msg_to_encrypt, RSA *public_key, char **encrypted_msg);

int decrypt_message(char *encrypted_msg, RSA *keys, char **plain_text_msg);

int get_max_number_of_bytes_auth(RSA *key);

int get_RSA_size(RSA *key);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
