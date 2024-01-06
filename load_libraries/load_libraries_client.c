#include <dlfcn.h>
#include <stdio.h>
#include "../constants.h"
#include <stdlib.h>
#include "../client.h"

static void (*snd_msg)(char msg[MESSAGE_SIZE], int port);

static void *libraryHandle;

void loadLibrary_client() {
    if (libraryHandle != NULL && snd_msg != NULL) return;
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

void unloadLibrary_client() { // TODO : needs to do a refactor to include the unloading of libraries
    dlclose(libraryHandle);
}

int send_message(char msg[MESSAGE_SIZE], int port) {
    loadLibrary_client();
    if (port != SERVER_PORT && port != CLIENT_PORT) return -1;
    snd_msg(msg, port);
    return 0;
}
