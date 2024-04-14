#ifndef SOFTSEC_PROJECT_SECTRANS_AUTH_H

#include "../utils/bool_type.h"

#define SOFTSEC_PROJECT_SECTRANS_AUTH_H

#define MAX_CHAR_ONE_LINE_USERS 41 // 40 chars + ';' // TODO: supprimer ces commentaires avant livraison

boolean authenticate(char *username, char *password);

#endif //SOFTSEC_PROJECT_SECTRANS_AUTH_H
