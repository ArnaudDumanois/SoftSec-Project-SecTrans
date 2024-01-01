#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "../server.h"

static void (*startserv)(int port);

static void (*stopserv)();

static void (*get_msg)(char message[1024]);

static void *libraryHandle;

void loadLibrary_server() {
    if (libraryHandle != NULL && startserv != NULL && stopserv != NULL && get_msg != NULL) return;
    libraryHandle = dlopen("../dynlib/libserver.so", RTLD_LAZY);
    if (!libraryHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    startserv = dlsym(libraryHandle, "startserver");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    stopserv = dlsym(libraryHandle, "stopserver");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    get_msg = dlsym(libraryHandle, "getmsg");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    printf("Alright !\n");
}

void unloadLibrary_server() {
    dlclose(libraryHandle);
}

int start_server(int port) {
    printf("toto");
    loadLibrary_server();
    startserv(port);
    return 0;
}

int stop_server() {
    stopserv();
    unloadLibrary_server();
    return 0;
}

int get_message(char msg_read[1024]) {
    get_msg(msg_read);
    return 0;
}