#include "common_message_management.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "../actions.h"

void add_action_at_index(char msg_to_send[MESSAGE_SIZE], char action, int index) {
    msg_to_send[index] = action;
    msg_to_send[index + 1] = ';';
}

void add_action(char msg_to_send[MESSAGE_SIZE], char action) {
    add_action_at_index(msg_to_send, action, 0);
}

void add_filename(char msg_to_send[MESSAGE_SIZE], char filename[]) { // TODO : checks to do to not exceed array's size
    if (msg_to_send[1] == ';') {
        strcat(msg_to_send, filename);
        msg_to_send[2 + strlen(filename)] = ';';
    }
}

void insert_between_semicolons(char *input, const char *string) {
    // Allouer de la mémoire pour la nouvelle chaîne
    size_t new_length = strlen(string);
    size_t input_length = strlen(input);
    size_t total_length = input_length + new_length + 2;  // +2 pour le point-virgule et le caractère nul
    char *result = (char *)malloc(total_length);

    if (result != NULL) {
        // Copier la chaîne d'origine
        strcpy(result, input);

        // Concaténer la nouvelle action
        strcat(result, string);

        // Ajouter un point-virgule à la fin
        strcat(result, ";");

        // Copier le résultat dans la chaîne d'origine
        strcpy(input, result);

        // Libérer la mémoire allouée
        free(result);
    } else {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
}

void extract_between_semicolons_at_index(const char *input, int index, char *output, size_t output_size) {
    const char *start = input;
    const char *end;

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
    if (length >= output_size + 1) {
        fprintf(stderr, "Output buffer too small.\n");
        return;
    }

    strncpy(output, start, length);
    output[length] = '\0';  // Ajouter le terminateur de chaîne nulle

    printf("INFORMATION EXTRACT : %s\n", output);
}

size_t total_size_between_semicolons(const char *input, int start_index) {
    const char *start = input;

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

    printf("TOTAL SIZE FOUND : %ld\n", end - start);

    // Calculer la taille totale des éléments entre les délimiteurs
    return (size_t) (end - start);
}

int is_action(const char received_action, const char existing_action) {
    return received_action == existing_action;
}

int is_valid_action(const char action) {
    return (
            is_action(action, ACTION_CREATE) ||
            is_action(action, ACTION_ADD) ||
            is_action(action, ACTION_DOWNLOAD) ||
            is_action(action, ACTION_REMOVE) ||
            is_action(action, ACTION_END) ||
            is_action(action, ACTION_LIST) ||
            is_action(action, ACTION_LOGIN) ||
            is_action(action, ACTION_REGISTER) ||
            is_action(action, ACTION_REPONSE)
    );
}

void add_string(char msg_to_send[MESSAGE_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if (msg_to_send[1] == ';') {
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}

void
add_string_without_dot(char msg_to_send[MESSAGE_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if (msg_to_send[1] == ';') {
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}