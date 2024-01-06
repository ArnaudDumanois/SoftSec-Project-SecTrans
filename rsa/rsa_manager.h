#ifndef SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
#define SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H

char *get_ciphertext_from_plaintext(char *plaintext, int length_plaintext, char *public_key);

char *get_plaintext_from_ciphertext(char *ciphertext, int length_ciphertext, char *private_key);

#endif //SOFTSEC_PROJECT_SECTRANS_RSA_MANAGER_H
