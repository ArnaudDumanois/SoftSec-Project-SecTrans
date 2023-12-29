#include "io_client.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MB (1024*1024)
#define MAX_ACCEPTED_FILE_SIZE (10*MB)

char *read_entire_file(int fd) {
    struct stat file_info;

    if (fstat(fd, &file_info) == -1) {
        perror("Erreur lors de l'obtention des informations sur le fichier\n");
        return NULL;
    }

    size_t buffer_size = file_info.st_size;

    if (buffer_size > MAX_ACCEPTED_FILE_SIZE) {
        perror("Le fichier que vous tentez d'envoyer est trop gros\n");
        close(fd);
        return NULL;
    }

    char *buffer = (char *) malloc(buffer_size);
    char *beginning = buffer;

    if (buffer == NULL) {
        perror("Erreur lors de l'allocation de mémoire\n");
        close(fd);
        return NULL;
    }

    if (read(fd, buffer, buffer_size) != buffer_size) {
        printf("Erreur lors de la lecture");
        free(buffer);
        close(fd);
        return NULL;
    }

    return beginning;
}