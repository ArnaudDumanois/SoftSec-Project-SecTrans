#include "server.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "constants.h"
#include "file_management/file_management_server.h"

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

int startserver(int port) {
    start_server(port);
    return 0;
}

int stopserver() {
    stop_server();
    return 0;
}

int main(int argc, char *argv[]) {
    loadLibrary_server();

    // Démarrage du serveur
    startserver(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    char message[INPUT_SIZE];
    while (1) {
        get_msg(message);
        printf("Message reçu du client : %s\n", message);

        // Exemple de condition pour arrêter le serveur
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }
    }

    // Arrêt du serveur
    stopserver();
    printf("Serveur arrêté\n");

    unloadLibrary_server();

    return 0;
}

