#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "../constants.h"
#include "load_libraries_server.h"

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
    printf("Alright !\n");
}

void unloadLibrary_server() {
    dlclose(libraryHandle);
}

void call_start_server(int port) {
    start_server(port);
}

void call_stop_server() {
    stop_server();
}

void call_get_msg_server(char message[1024]) {
    get_msg(message);
}