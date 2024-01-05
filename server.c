#include <stdio.h>
#include "constants.h"
#include "server.h"
#include "message_management/server_message_management/server_message_management.h"
#include "rsa/rsa.h"

int main() {
    // Démarrage du serveur
    start_server(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    char message[INPUT_SIZE];
    char *private_key;
    char *public_key;
    generate_keys(&private_key, &public_key);
    printf("%s\n", private_key);
    printf("%s\n", public_key);
    listen_message(message);

    // Arrêt du serveur
    stop_server();
    printf("Serveur arrêté\n");

    return 0;
}

