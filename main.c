//
// Created by arnau on 20/12/2023.
//
#include <stdio.h>
#include "client.h"
#include "server.h"

int main() {
    // Initialisation du serveur
    int port = 12345;  // Choisissez le port selon vos besoins
    int serverStatus = startserver(port);

    if (serverStatus == 0) {
        printf("Serveur démarré avec succès sur le port %d.\n", port);

        // Envoi d'un message depuis le client
        char messageToSend[1024] = "Bonjour, serveur !";
        int sendStatus = sndmsg(messageToSend, port);

        if (sendStatus == 0) {
            printf("Message envoyé avec succès depuis le client.\n");

            // Lecture du message côté serveur
            char receivedMessage[1024];
            int receiveStatus = getmsg(receivedMessage);

            if (receiveStatus == 0) {
                printf("Message reçu côté serveur : %s\n", receivedMessage);
            } else {
                printf("Erreur lors de la lecture du message côté serveur.\n");
            }
        } else {
            printf("Erreur lors de l'envoi du message depuis le client.\n");
        }

        // Arrêt du serveur
        int stopServerStatus = stopserver();
        if (stopServerStatus == 0) {
            printf("Serveur arrêté avec succès.\n");
        } else {
            printf("Erreur lors de l'arrêt du serveur.\n");
        }
    } else {
        printf("Erreur lors du démarrage du serveur.\n");
    }

    return 0;
}
