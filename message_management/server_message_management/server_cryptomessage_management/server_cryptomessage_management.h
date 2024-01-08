#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_CRYPTOMESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_CRYPTOMESSAGE_MANAGEMENT_H

#include "../../../constants.h"

unsigned char *decode_msg(char msg[INPUT_SIZE]);

unsigned char *read_rsa_public_key(char msg[INPUT_SIZE]);

int send_aes_ciphered_rsa_public_key(unsigned char *public_key, char msg_to_send[INPUT_SIZE]);

#endif //SOFTSEC_PROJECT_SECTRANS_SERVER_CRYPTOMESSAGE_MANAGEMENT_H
