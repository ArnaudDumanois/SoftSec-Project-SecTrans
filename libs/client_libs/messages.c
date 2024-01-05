//
// Created by Julien on 03/01/2024.
//

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "messages.h"

char *getManualForSpecificCommand(char *command) {
    char *message = malloc(512);  // Assurez-vous d'allouer suffisamment de mémoire
    if (message == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(message, "");  // Utilisez strcpy pour copier le contenu initial

    if (strncmp(command, "-list", 5) == 0) {
        strcat(message, "MANUAL FOR ");
        strcat(message, command);
        strcat(message, "\n");
        //PRINT LE RESTE DU MANUEL
    } else if (strncmp(command, "-up", 3) == 0) {
        strcat(message, "MANUAL FOR ");
        strcat(message, command);
        strcat(message, "\n");
        //PRINT LE RESTE DU MANUEL
    } else if (strncmp(command, "-down", 5) == 0) {
        strcat(message, "MANUAL FOR ");
        strcat(message, command);
        strcat(message, "\n");
        //PRINT LE RESTE DU MANUEL
    } else if (strncmp(command, "-login", 6) == 0) {
        strcat(message, "MANUAL FOR -> ");
        strcat(message, command);
        strcat(message, "\n");
        //PRINT LE RESTE DU MANUEL
    }
    return message;
}
