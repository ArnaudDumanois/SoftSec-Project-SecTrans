#include "server.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "file_management/file_management_server.h"
#include "file_management/file_management.h"
#include "actions.h"

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

void manage_file(char action, char filename[], char file_content_to_write[]) {
    if (action == ACTION_CREATE) {
        if (does_file_exist(filename) == TRUE) {
            // get fd and write
        } else {
            int fd = create_file(filename);
            int nb_bytes_wrote = write_file_content(fd, file_content_to_write, strlen(file_content_to_write));
            printf("%d\n", nb_bytes_wrote);
        }
    }
}

void manage_request(char message[INPUT_SIZE]) {
    char action = message[0];
    char *filename = get_filename_from_message(message);
    manage_file(action, filename, get_file_content_from_message(message));
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
        manage_request(message);

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

