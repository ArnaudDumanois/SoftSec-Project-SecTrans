//
// Created by arnau on 20/12/2023.
//

#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

void loadLibrary_server();
void unloadLibrary_server();

int startserver(int port);
int stopserver();

/* read message sent by client */
int getmsg(char msg_read[1024]);
#endif //SOFTSEC_PROJECT_SECTRANS_SERVER_H
