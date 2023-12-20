//
// Created by arnau on 20/12/2023.
//

#include "server.h"

// lib du .so
static void (*start_server)(int port);
static void (*stop_server)();
static void (*get_msg)(char message[1024]);

static void *libraryHandle;

void loadLibrary_server() {
    libraryHandle = dlopen("libserver.so", RTLD_LAZY);
    if (!libraryHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    start_server = dlsym(libraryHandle, "startserver");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    stop_server = dlsym(libraryHandle, "stopserver");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    get_msg = dlsym(libraryHandle, "getmsg");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unloadLibrary_server() {
    dlclose(libraryHandle);
}

int startserver(int port) {
    start_server(port);
    return port;
}

int stopserver() {
    stop_server();
    return 0;
}

int getmsg(char msg_read[1024]) {
    get_msg(msg_read);
    return 0;
}

