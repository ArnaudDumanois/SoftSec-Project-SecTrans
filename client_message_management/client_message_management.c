#include "../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_management/file_management.h"
#include <unistd.h>
#include "../load_libraries/load_libraries_client.h"
#include "../utils/array_utils.h"
#include "../actions.h"
#include "../server_message_management/server_message_management.h"

#define STILL 1
#define END 0

int sndmsg(char msg[INPUT_SIZE], int port) {
    if (port != SERVER_PORT) return -1;
    call_send_msg(msg, port);
    return 0;
}

int get_next_blocks_file(char **current_pointer_to_file_content, char msg_to_send[], int nb_max_bytes_to_read) {
    int i = 0;
    printf("%p\n", *current_pointer_to_file_content);
    while (i < nb_max_bytes_to_read) {
        if (*(*current_pointer_to_file_content) == '\0') {
            return END;
        }
        msg_to_send[i] = *(*current_pointer_to_file_content);
        i++;
        (*current_pointer_to_file_content)++;
    }
    return *(*current_pointer_to_file_content) == '\0' ? END : STILL;
}

void add_action(char msg_to_send[INPUT_SIZE], char action) {
    msg_to_send[0] = action;
    msg_to_send[1] = ';';
}

void add_filename(char msg_to_send[INPUT_SIZE], char filename[]) {
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, filename);
        msg_to_send[2 + strlen(filename)] = ';';
    }
}

int
get_current_msg_to_send(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], char action) {
    add_action(msg_to_send, action);
    add_filename(msg_to_send, filename);

    size_t filename_length = strlen(filename);
    int nb_bytes_max_to_read = INPUT_SIZE - filename_length - 3; // for \0 at the end and the two ; in the middle
    // the filename begins at index 2
    return get_next_blocks_file(ptr_current_file_content, &msg_to_send[2 + filename_length + 1], nb_bytes_max_to_read);
}

void sending(char msg_to_send[INPUT_SIZE], int port) {
    printf("%s\n", msg_to_send);
    printf("%d\n", port);
    if (sndmsg(msg_to_send, port) != 0) {
        printf("Server can't receive your file - Port error");
        exit(EXIT_FAILURE);
    }
}

int
sending_common(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], char action, int port) {
    int result = get_current_msg_to_send(ptr_current_file_content, msg_to_send, filename, action);
    //printf("current msg_to_send : %s\n", msg_to_send);
    sending(msg_to_send, port);
    return result;
}

int first_send(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], int port) {
    return sending_common(ptr_current_file_content, msg_to_send, filename, ACTION_CREATE, port);
}

int mid_send(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], int port) {
    return sending_common(ptr_current_file_content, msg_to_send, filename, ACTION_ADD, port);
}

void final_send(char msg_to_send[INPUT_SIZE], char filename[], int port) {
    add_action(msg_to_send, ACTION_END);
    add_filename(msg_to_send, filename); // TODO : absolument vérifier la taille du nom du fichier avant
    strcat(msg_to_send, ";");
    sending(msg_to_send, port);
}

void send_file(char filepath[], int port) {
    int fd = open_file(filepath, 0); // 0 = read only
    char *file_content;
    size_t siz_file_content;
    if (readall(fd, &file_content, &siz_file_content) != 0) {
        printf("An error has happened when reading the file you want to send\n");
        exit(EXIT_FAILURE);
    }
    close(fd);
    char msg_to_send[INPUT_SIZE] = {'\0'};
    char *filename = get_file_name_from_filepath(filepath);
    int result = first_send(&file_content, msg_to_send, filename, port);
    clear_array(msg_to_send, INPUT_SIZE);
    while (result == STILL) {
        result = mid_send(&file_content, msg_to_send, filename, port);
        clear_array(msg_to_send, INPUT_SIZE);
    }
    final_send(msg_to_send, filename, port);
    clear_array(msg_to_send, INPUT_SIZE);
    free(file_content);
    printf("Message sent !\n");
}

void ask_for_file_to_server(char msg_to_send[INPUT_SIZE], int port, char filename[]) {
    add_action(msg_to_send, ACTION_DOWNLOAD);
    add_filename(msg_to_send, filename);
    sending(msg_to_send, port);
}

void download_file(char filename[], int port) {
    char msg_to_send[INPUT_SIZE] = {'\0'};
    ask_for_file_to_server(msg_to_send, port, filename);
    listen_message(msg_to_send);
}