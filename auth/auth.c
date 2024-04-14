#include "auth.h"
#include "../utils/bool_type.h"
#include <stdio.h>
#include <string.h>

boolean is_between_12_and_20_chars(char *str);

boolean contains_at_least_one_special_char(char *str);

boolean contains_at_least_one_number(char *str);

boolean authenticate(char *username, char *password) {
    char filename[] = "Users.txt";
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char line[MAX_CHAR_ONE_LINE_USERS];
        char *auth_field;
        char *search = ";";
        while (fgets(line, sizeof(line), file) != NULL) /* read a line from a file */ {
            auth_field = strtok(line, search); // username
            if (is_between_12_and_20_chars(auth_field)) {
                auth_field = strtok(NULL, search); // password
                if (is_between_12_and_20_chars(auth_field) && contains_at_least_one_number(auth_field)
                    && contains_at_least_one_special_char(auth_field)) {
                    return TRUE;
                }
            }
            return FALSE;
        }
        fclose(file);
    } else {
        perror("Your username/password have not the correct format"); //print the error message on stderr.
    }
    return TRUE;
}

boolean is_between_12_and_20_chars(char *str) {
    int i;
    for (i = 1; str != NULL; str++, i++) {
        if (i > 20) return FALSE;
    }
    if (i < 12) return FALSE;
    return TRUE;
}

boolean contains_at_least_one_special_char(char *str) {
    char special_chars[] = {'?', '!', '/', '^', '$', '@', '=', '+', '-'};
    for (char *c = str; c != NULL; c = str++) {
        for (int i = 0; special_chars[i] != '\0'; i++) {
            if (*c == special_chars[i]) return TRUE;
        }
    }
    return FALSE;
}

