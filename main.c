//
// Created by arnau on 20/12/2023.
//
#include <stdio.h>
#include "client.h"
#include "server.h"

int main() {
    // Initialisation du serveur
    int port = 1;  // Choisissez le port selon vos besoins
    printf("Démarrage du serveur avec le port %d...\n", port);
    int serverStatus = startserver(port);
    if (serverStatus == -1) {
        printf("Erreur lors du démarrage du serveur\n");
        return -1;
    }
}
