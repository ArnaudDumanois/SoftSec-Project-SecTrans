//
// Created by arnau on 20/12/2023.
//
#include <stdio.h>
#include "client.h"
#include "server.h"

int main() {
    loadLibrary_server();
    printf("Démarrage du serveur avec le 2000\n");
    int serverStatus = startserver(2000);
    printf("Serveur démarré avec le status %d\n", serverStatus);
    if (serverStatus == -1) {
        printf("Erreur lors du démarrage du serveur\n");
        return -1;
    }
}
