#include "server.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "file_management/file_management_server.h"
#include "actions.h"
#include "file_management/file_management.h"
#include <unistd.h>
#include <fcntl.h>

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

void manage_file(char action, char filename[],
                 char file_content_to_write[]) { // TODO : checks when write returns 0 byte written
    if (action == ACTION_CREATE || action == ACTION_ADD) {
        //int nb_bytes_wrote;
        //printf("DOES FILES EXIST : %d\n", does_file_exist(filename));
        if (does_file_exist(filename) == FALSE) {
            printf("filename to create : %s\n", filename);
            create_file(filename);
        }
        write_file_content(filename, file_content_to_write, strlen(file_content_to_write));
        //printf("nb bytes written : %d\n", nb_bytes_wrote);
    }
}

void clear_array(char array[], int nb_elements_to_clear) {
    for (int i = 0; i < nb_elements_to_clear; i++) {
        array[i] = '\0';
    }
}

void manage_request(char message[INPUT_SIZE]) {
    char action = message[0];
    char *filename = get_filename_from_message(message);
    char *file_content = get_file_content_from_message(message);
    //printf("Message content : %s\n", file_content);
    manage_file(action, filename, file_content);
    free(filename);
}

int main() {
    loadLibrary_server();

    // Démarrage du serveur
    startserver(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    char message[INPUT_SIZE];
    while (1) {
        get_msg(message);
        //printf("Message reçu du client : %s\n", message);
        manage_request(message);
        clear_array(message, INPUT_SIZE);

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

