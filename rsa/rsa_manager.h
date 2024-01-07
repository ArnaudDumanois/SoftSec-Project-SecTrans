#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H

#include "rsa.h"

RSA *generate_RSA_keys();

int encrypt_message(char *msg_to_encrypt, RSA *keys, char **encrypted_msg);

int decrypt_message(char *encrypted_msg, RSA *keys, char **plain_text_msg);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
