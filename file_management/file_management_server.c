#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file_management_server.h"
#include <unistd.h>

#define STORING_PATH "../files_to_store/"

int create_file(char *filename) {
    unsigned int filename_length = strnlen(filename, MAX_FILEPATH_SIZE + 1);
    unsigned int storing_path_length = strlen(STORING_PATH);
    if (filename_length == MAX_FILEPATH_SIZE + 1) {
        printf("Filepath is too long\n");
        return -1;
    }
    char *filepath = (char *) malloc(sizeof(char) * (storing_path_length + filename_length + 1)); // + 1 for \0
    strncpy(filepath, STORING_PATH, storing_path_length);
    int fd = open(strncat(filepath, filename, strnlen(filename, MAX_FILEPATH_SIZE)), O_CREAT | O_WRONLY);
    free(filepath);
    return fd;
}

int write_file_content(int fd, char file_content[INPUT_SIZE]) {
    int nb_bytes_wrote;
    if ((nb_bytes_wrote = write(fd, file_content, INPUT_SIZE)) == -1) {
        printf("Erreur lors de l'écriture dans un fichier\n");
        exit(EXIT_FAILURE);
    }
    return nb_bytes_wrote;
}