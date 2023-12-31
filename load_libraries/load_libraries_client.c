#include <dlfcn.h>
#include <stdio.h>
#include "../constants.h"
#include <stdlib.h>

static void (*snd_msg)(char msg[INPUT_SIZE], int port);

static void *libraryHandle;

void loadLibrary_client() {
    libraryHandle = dlopen("../dynlib/libclient.so", RTLD_LAZY);
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

void unloadLibrary_client() {
    dlclose(libraryHandle);
}

void call_send_msg(char msg[1024], int port) {
    snd_msg(msg, port);
}
