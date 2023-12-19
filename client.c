//
// Created by arnau on 20/12/2023.
//

#include "client.h"

static void (*snd_msg)(char msg[1024], int port);

static void *libraryHandle;

void loadLibrary() {
    libraryHandle = dlopen("libserver.so", RTLD_LAZY);
    if (!libraryHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    snd_msg = dlsym(libraryHandle, "sndmsg");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unloadLibrary() {
    dlclose(libraryHandle);
}


int sndmsg(char msg[1024], int port) {
    snd_msg(msg, port);
    return 0;
}