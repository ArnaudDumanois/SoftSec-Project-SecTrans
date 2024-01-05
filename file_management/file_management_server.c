#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file_management_server.h"
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FILENAME_SIZE 512

char *get_complete_filepath(char *filename, char *path_to_use) {
    printf("START COMPLETE_FILE_PATH\n");
    char *filepath = malloc(strlen(path_to_use) + strlen(filename) + 1);
    strcpy(filepath, path_to_use);
    strcat(filepath, filename);
    printf("END COMPLETE_FILE_PATH\n");
    return filepath;
}

char *get_complete_filepath_storing(char *filename) {
    return get_complete_filepath(filename, STORING_PATH);
}

char *get_complete_filepath_getting(char *filename) {
    return get_complete_filepath(filename, GETTING_PATH);
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
    printf("BEFORE GET_COMPLETE_FILEPATH\n");
    char *filepath = get_complete_filepath_storing(filename);
    printf("%s\n",filepath);

    struct stat st = {0};
    if (stat(STORING_PATH, &st) == -1) {
        mkdir(STORING_PATH, 0700);
    }

    printf("AFTER GET_COMPLETE_FILEPATH\n");

    // LEAK MEMOIRE INDIQUE PAR CLION MAIS C'EST A CAUSE DU FLAG O_SYNC NON RECONNU SUR WINDOWS ET Y A RIEN TQT
    // Create a file
    fopen(filepath, "w");
    printf("file creating\n");
    //free the filepath
    free(filepath);
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

char *get_first_string(char msg[INPUT_SIZE]) {
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
    for(int j=0;j<sizeof(filename);j++){
        printf("%c",filename[i]);
    }
    return filename;
}

char *get_second_string(char msg[INPUT_SIZE]) {
    char *ptr2 = strchr(&msg[3], ';'); // to get the second ;
    char *tok = strtok(ptr2 + 1, "\0");
    return tok;
}

