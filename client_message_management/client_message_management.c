#include "../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_management/file_management.h"
#include <unistd.h>
#include "../load_libraries/load_libraries_client.h"
#include "../utils/array_utils.h"

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

int
get_current_msg_to_send(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], char action) {
    msg_to_send[0] = action;
    msg_to_send[1] = ';';
    strcat(msg_to_send, filename);
    size_t filename_length = strlen(filename);
    msg_to_send[2 + filename_length] = ';'; // the filename begins at index 2

    int nb_bytes_max_to_read = INPUT_SIZE - filename_length - 3; // for \0 at the end and the two ; in the middle

    return get_next_blocks_file(ptr_current_file_content, &msg_to_send[2 + filename_length + 1], nb_bytes_max_to_read);
}

void sending(char msg_to_send[INPUT_SIZE], int port) {
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
    return sending_common(ptr_current_file_content, msg_to_send, filename, 'S', port);
}

int mid_send(char **ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], int port) {
    return sending_common(ptr_current_file_content, msg_to_send, filename, 'A', port);
}

void final_send(char msg_to_send[INPUT_SIZE], char filename[], int port) {
    strcpy(msg_to_send, "E;");
    strcat(msg_to_send, filename); // TODO : absolument vérifier la taille du nom du fichier avant
    strcat(msg_to_send, ";");
    sending(msg_to_send, port);
}

void send_file(char filepath[], int port) {
    int fd = open_file(filepath, 0); // 0 = read only
    char *file_content;
    size_t siz_file_content;
    readall(fd, &file_content, &siz_file_content);
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
    printf("Message sent !\n");
}