#ifndef SOFTSEC_PROJECT_SECTRANS_AUTH_H
#define SOFTSEC_PROJECT_SECTRANS_AUTH_H

#define MAX_CHAR_ONE_LINE_USERS 41 // 40 chars + ';' // TODO: supprimer ces commentaires avant livraison
typedef enum {
    false, true
} bool;

bool authenticate(char *username, char *password);

#endif //SOFTSEC_PROJECT_SECTRANS_AUTH_H
