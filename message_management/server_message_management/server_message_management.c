#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../file_management/file_management.h"
#include "../../file_management/file_management_server.h"
#include "../../actions.h"
#include "../../utils/array_utils.h"
#include "../client_message_management/client_message_management.h"
#include "../../server.h"
#include <dirent.h>
#include <unistd.h>
#include "../common_message_management.h"
#include "../../libs/logger/logger_sha.h"

void list_files(char msg_to_send[INPUT_SIZE]) { // TODO : to finish
    DIR *d;
    struct dirent *dir;
    d = opendir(STORING_PATH);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            add_filename(msg_to_send, dir->d_name);
        }
        closedir(d);
    }
}

void manage_file(const char *action, char filename[],
                 char file_content_to_write[]) { // TODO : checks when write returns 0 byte written, and send stop to client
    if (strcmp(action,ACTION_CREATE)==0 || strcmp(action,ACTION_ADD)==0) {
        printf("ACTION_CREATE\n");
        //int nb_bytes_wrote;
        //printf("DOES FILES EXIST : %d\n", does_file_exist(filename));
        if (does_file_exist(filename) == FALSE) {
            printf("filename to create : %s\n", filename);
            create_file(filename);
        }
        write_file_content(filename, file_content_to_write, strlen(file_content_to_write));
        //printf("nb bytes written : %d\n", nb_bytes_wrote);
    } else if (strcmp(action,ACTION_END)==0) {
        printf("The file has been created !\n");
    }
}

char *get_passwd(char *message) {
    char *cut = strtok(message, ";");
    return &cut[2];
}

char *get_username(const char *message) {
    char *b = strchr(message, ';');
    char *e = strchr(b, ';');
    char *username = malloc(e - b + 1);
    int j = 0;
    for (char *c = b; c != e; c++) {
        username[j] = *c;
        j++;
    }
    username[j] = '\0';
    return username;
}

void manage_request(char message[INPUT_SIZE]) {
    printf("MANAGE REQUEST\n");
    char *action = strtok(message,";");

    if (strcmp(action,ACTION_CREATE)==0 || strcmp(action,ACTION_ADD)==0 || strcmp(action,ACTION_END)==0) {
        char *filename = get_first_string(message);
        char *file_content = get_second_string(message);
        //printf("Message content : %s\n", file_content);
        manage_file(action, filename, file_content);
        free(filename);
        clear_array(message, INPUT_SIZE);
    }
    else if (strcmp(action,ACTION_DOWNLOAD)==0) {
        char *filename = get_first_string(message);
        char *filepath = get_complete_filepath_storing(filename);
        printf("%s\n", filepath);
        send_file(filepath, CLIENT_PORT);
        free(filename);
        free(filepath);
        clear_array(message, INPUT_SIZE);
    }
    else if (strcmp(action,ACTION_LOGIN)==0) {
        printf("LOGIN ACTION !\n");
        //vérifier si user existe
        int res = authenticate_user(get_first_string(message), get_second_string(message));
        if (res == 1) { printf("Auth Réussie"); }
        else { printf("Auth échouée"); }
    }
    else if (strcmp(action,ACTION_REGISTER)==0){
        //vérifier si user existe
        int res = authenticate_user(get_first_string(message), get_second_string(message));
        if (res == 1) { printf("Le compte existe déjà !"); }
        else {
            printf("Création du compte...");
            User newUser;
            strcpy(newUser.username, "utilisateur1");
            generate_salt(newUser.salt);
            hash_password("motdepasse123", newUser.salt, newUser.hashed_password);
            save_user(&newUser);

        }
    }
}

void listen_message(char message[INPUT_SIZE]) {
        get_message(message);
        printf("Message reçu : %s\n", message);
        manage_request(message);
}