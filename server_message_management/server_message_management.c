#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_management/file_management_server.h"
#include "../actions.h"
#include "../utils/array_utils.h"

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
    char *filename = get_filename_from_message(message);
    char *file_content = get_file_content_from_message(message);
    //printf("Message content : %s\n", file_content);
    manage_file(action, filename, file_content);
    free(filename);
    clear_array(message, INPUT_SIZE);
}
