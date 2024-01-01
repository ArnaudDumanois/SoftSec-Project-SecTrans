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
#include "../common_message_management.h"

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

void manage_file(char action, char filename[],
                 char file_content_to_write[]) { // TODO : checks when write returns 0 byte written, and send stop to client
    if (action == ACTION_CREATE || action == ACTION_ADD) {
        //int nb_bytes_wrote;
        //printf("DOES FILES EXIST : %d\n", does_file_exist(filename));
        if (does_file_exist(filename) == FALSE) {
            printf("filename to create : %s\n", filename);
            create_file(filename);
        }
        write_file_content(filename, file_content_to_write, strlen(file_content_to_write));
        //printf("nb bytes written : %d\n", nb_bytes_wrote);
    } else if (action == ACTION_END) {
        printf("The file has been created !\n");
    }
}

void manage_request(char message[INPUT_SIZE]) {
    char action = message[0];
    if (action == ACTION_CREATE || action == ACTION_ADD || action == ACTION_END) {
        char *filename = get_filename_from_message(message);
        char *file_content = get_file_content_from_message(message);
        //printf("Message content : %s\n", file_content);
        manage_file(action, filename, file_content);
        free(filename);
        clear_array(message, INPUT_SIZE);
    } else if (action == ACTION_DOWNLOAD) {
        char *filename = get_filename_from_message(message);
        char *filepath = get_complete_filepath_storing(filename);
        printf("%s\n", filepath);
        send_file(filepath, CLIENT_PORT);
        free(filename);
        free(filepath);
        clear_array(message, INPUT_SIZE);
    }
}

void listen_message(char message[INPUT_SIZE]) {
    while (1) {
        get_message(message);
        printf("Message reçu du client : %s\n", message);
        manage_request(message);

        // Exemple de condition pour arrêter le serveur
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }
    }
}