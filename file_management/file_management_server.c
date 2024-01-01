#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file_management_server.h"
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FILENAME_SIZE 512

char *get_complete_filepath_storing(char *filename) {
    char *filepath = malloc(strlen(STORING_PATH) + strlen(filename) + 1);
    strcpy(filepath, STORING_PATH);
    strcat(filepath, filename);
    return filepath;
}

char *get_complete_filepath_getting(char *filename) {
    char *filepath = malloc(strlen(GETTING_PATH) + strlen(filename) + 1);
    strcpy(filepath, GETTING_PATH);
    strcat(filepath, filename);
    return filepath;
}

struct stat *get_struct_stat_of_file(char *filename) {
    struct stat *stats = malloc(sizeof(struct stat));
    char *filepath = get_complete_filepath_storing(filename);
    boolean is_stat_ok = FALSE;
    if (stat(filepath, stats) == 0) {
        is_stat_ok = TRUE;
    }
    free(filepath);
    return is_stat_ok ? stats : NULL;
}

boolean does_file_exist(char *filename) {
    struct stat *stats = get_struct_stat_of_file(filename);
    boolean does_file_exist = FALSE;
    if (stats != NULL) {
        does_file_exist = TRUE;
        free(stats);
    }
    return does_file_exist;
}

void create_file(char *filename) {
    size_t filename_length = strnlen(filename, MAX_FILENAME_SIZE + 1);
    if (filename_length == MAX_FILENAME_SIZE + 1) {
        printf("Filename is too long\n");
    }
    char *filepath = get_complete_filepath_storing(filename);
    // LEAK MEMOIRE INDIQUE PAR CLION MAIS C'EST A CAUSE DU FLAG O_SYNC NON RECONNU SUR WINDOWS ET Y A RIEN TQT
    int fd = open(filepath, O_CREAT | O_EXCL | O_SYNC | O_WRONLY, S_IWUSR);
    free(filepath);
    close(fd);
}

int write_file_content(char *filename, char file_content[], size_t nb_char_to_write) {
    char *filepath = get_complete_filepath_storing(filename);
    int fd = open(filepath, O_SYNC | O_WRONLY | O_APPEND, S_IWUSR);
    free(filepath);
    if (fd == -1) {
        return -1;
    }
    int nb_bytes_wrote;
    if ((nb_bytes_wrote = write(fd, file_content, nb_char_to_write)) == -1) {
        perror("Erreur :");
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
    char *ptr2 = strchr(&msg[3], ';'); // to get the second ;
    char *tok = strtok(ptr2 + 1, "\0");
    return tok;
}

