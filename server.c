#include "server.h"
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "load_libraries/load_libraries_server.h"
#include "server_message_management/server_message_management.h"

int startserver(int port) {
    call_start_server(port);
    return 0;
}

int stopserver() {
    call_stop_server();
    return 0;
}

int getmsg(char msg_read[1024]) {
    call_get_msg_server(msg_read);
    return 0;
}

int main() {
    loadLibrary_server();

    // Démarrage du serveur
    startserver(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    char message[INPUT_SIZE];
    while (1) {
        getmsg(message);
        //printf("Message reçu du client : %s\n", message);
        manage_request(message);

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

