#ifndef SOFTSEC_PROJECT_SECTRANS_CLIENT_H
#define SOFTSEC_PROJECT_SECTRANS_CLIENT_H

#include "constants.h"

/* send message (maximum size: 1024 bytes) */
int sndmsg(char msg[INPUT_SIZE], int port);

#endif //SOFTSEC_PROJECT_SECTRANS_CLIENT_H
