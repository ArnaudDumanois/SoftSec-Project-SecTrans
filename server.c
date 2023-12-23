#include "server.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT_SERVEUR 2000

// lib du .so
static void (*start_server)(int port);

static void (*stop_server)();

static void (*get_msg)(char message[1024]);

static void *libraryHandle;

void loadLibrary_server() {
    libraryHandle = dlopen("../dynlib/libserver.so", RTLD_LAZY);
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

int main(int argc, char *argv[]) {
    loadLibrary_server();
    start_server(PORT_SERVEUR);
    printf("Hello Server !\n");
}

