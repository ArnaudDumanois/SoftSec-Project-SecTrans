#include "io_client.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define MB (1024*1024)
#define MAX_ACCEPTED_FILE_SIZE (10*MB)


#include <errno.h>

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
     (*dataptr) points to a dynamically allocated buffer, with
     (*sizeptr) chars read from the file.
     The buffer is allocated for one extra char, which is NUL,
     and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
*/
int readall(int fd, char **dataptr, size_t *sizeptr) {
    char *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (fd == -1 || dataptr == NULL || sizeptr == NULL)
        return READALL_INVALID;

    while (1) {
        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            // Overflow check
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }

            temp = realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }

        n = read(fd, data + used, READALL_CHUNK);
        if (n == 0) break;

        used += n;
    }

    temp = realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';

    *dataptr = data;
    *sizeptr = used;

    return READALL_OK;
}

/*
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
}*/