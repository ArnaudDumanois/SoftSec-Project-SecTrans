//
// Created by arnau on 20/12/2023.
//

#ifndef SOFTSEC_PROJECT_SECTRANS_CLIENT_H
#define SOFTSEC_PROJECT_SECTRANS_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void unloadLibrary_client();
void loadLibrary_client();

/* send message (maximum size: 1024 bytes) */
int sndmsg(char msg[1024], int port);

#endif //SOFTSEC_PROJECT_SECTRANS_CLIENT_H
