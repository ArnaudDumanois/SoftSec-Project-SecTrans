#include <stdio.h>
#include "constants.h"
#include "server.h"
#include "message_management/server_message_management/server_message_management.h"

int main() {
    // Démarrage du serveur
    start_server(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions
    while(1){
        char message[MESSAGE_SIZE];
        listen_message(message);
    }

    // Arrêt du serveur
    stop_server();
    printf("Serveur arrêté\n");

    return 0;
}

