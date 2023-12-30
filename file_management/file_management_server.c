#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file_management_server.h"
#include <unistd.h>
#include <sys/stat.h>

char *get_complete_filepath(char *filename) {
    char *filepath = malloc(strlen(STORING_PATH) + strlen(filename));
    strcpy(filepath, STORING_PATH);
    strcat(filepath, filename);
    return filepath;
}

struct stat *get_struct_stat_of_file(char filename[]) {
    struct stat *stats = malloc(sizeof(struct stat));
    char *filepath = get_complete_filepath(filename);
    boolean is_stat_ok = FALSE;
    if (stat(filepath, stats) == 0) {
        is_stat_ok = TRUE;
    }
    free(filepath);
    return is_stat_ok ? stats : NULL;
}

boolean does_file_exist(char filename[]) {
    struct stat *stats = get_struct_stat_of_file(filename);
    boolean does_file_exist = FALSE;
    if (stats != NULL) {
        does_file_exist = TRUE;
    }
    free(stats);
    return does_file_exist;
}

int create_file(char *filename) {
    unsigned int filename_length = strnlen(filename, MAX_FILEPATH_SIZE + 1);
    unsigned int storing_path_length = strlen(STORING_PATH);
    if (filename_length == MAX_FILEPATH_SIZE + 1) {
        printf("Filepath is too long\n");
        return -1;
    }
    char *filepath = (char *) malloc(sizeof(char) * (storing_path_length + filename_length + 1)); // + 1 for \0
    strncpy(filepath, STORING_PATH, storing_path_length);
    printf("ici on crée un fichier !\n");
    int fd = open(strncat(filepath, filename, strnlen(filename, MAX_FILEPATH_SIZE)), O_CREAT | O_WRONLY);
    free(filepath);
    return fd;
}

int write_file_content(int fd, char file_content[], size_t nb_char_to_write) {
    int nb_bytes_wrote;
    if ((nb_bytes_wrote = write(fd, file_content, nb_char_to_write + 1)) == -1) {
        printf("Erreur lors de l'écriture dans un fichier\n");
        exit(EXIT_FAILURE);
    }
    return nb_bytes_wrote;
}

char *get_filename_from_message(char msg[INPUT_SIZE]) {
    char *ptr = strchr(msg, ';');
    char *ptr2 = strchr(ptr + 1, ';');
    size_t filename_length = strlen(ptr + 1) - strlen(ptr2);
    char *filename = (char *) malloc(sizeof(char) * (filename_length + 1));
    int i = 0;
    for (char *p = ptr + 1; p != ptr2; p++) {
        filename[i] = *p;
        i++;
    }
    filename[i] = '\0';
    return filename;
}

char *get_file_content_from_message(char msg[INPUT_SIZE]) {
    char *ptr2 = strrchr(msg, ';');
    char *tok = strtok(ptr2 + 1, "\0");
    return tok;
}

