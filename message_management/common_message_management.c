#include "common_message_management.h"
#include <string.h>
#include <stdio.h>

void add_action(char msg_to_send[INPUT_SIZE], char *action) {
    size_t lenAction = strlen(action);
    for(int i=0;i<lenAction;i++){msg_to_send[i]=action[i];}
    msg_to_send[lenAction]=';';
}

void add_filename(char msg_to_send[INPUT_SIZE], char filename[]) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, filename);
        msg_to_send[2 + strlen(filename)] = ';';
    }
}

void extract_between_semicolons_at_index(const char *input, int index, char *output, size_t output_size) {
    const char *start = input;
    const char *end = input;

    // Trouver le début du N-ème élément
    for (int i = 0; i < index; ++i) {
        start = strchr(start, ';');
        if (start == NULL) {
            fprintf(stderr, "Index out of bounds.\n");
            return;
        }
        start += 1;  // Passer le point-virgule
    }

    // Trouver la fin du N+1-ème élément
    end = strchr(start, ';');
    if (end == NULL) {
        fprintf(stderr, "Next semicolon not found.\n");
        return;
    }

    size_t length = end - start;
    if (length >= output_size) {
        fprintf(stderr, "Output buffer too small.\n");
        return;
    }

    strncpy(output, start, length);
    output[length] = '\0';  // Ajouter le terminateur de chaîne nulle
}

size_t total_size_between_semicolons(const char *input, int start_index) {
    const char *start = input;
    printf("TOTAL SIZE FOR :\n %s\n",start);

    // Trouver le début du N-ème élément
    for (int i = 0; i < start_index; ++i) {
        start = strchr(start, ';');
        if (start == NULL) {
            fprintf(stderr, "Index out of bounds.\n");
            return 0;
        }
        start += 1;  // Passer le point-virgule
    }

    // Trouver la fin du N+1-ème élément
    const char *end = strchr(start, ';');
    if (end == NULL) {
        fprintf(stderr, "Next semicolon not found.\n");
        return 0;
    }

    // Calculer la taille totale des éléments entre les délimiteurs
    return (size_t)(end - start);
}

void add_string(char msg_to_send[INPUT_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}

void add_string_without_dot(char msg_to_send[INPUT_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}