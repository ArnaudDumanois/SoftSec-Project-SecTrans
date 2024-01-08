#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "../utils/util.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <regex.h>
#include "file_management.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MB (1024*1024)
#define MAX_ACCEPTED_FILE_SIZE (10*MB)

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

#define PATH_STORAGE_MAX 1024

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

int get_size(char *file_path) {
    struct stat *buf = malloc(sizeof(struct stat));
    if ((stat(file_path, buf) != -1)) {
        return buf->st_size;
    }
    return 0;
}

int get_total_size(char *file_path) {
    if (is_dir(file_path)) {
        DIR *dir = opendir(file_path);
        if (!dir) return 0; // mieux vaut retourner 0 et ne pas comptabiliser le fichier qui n'a pas pu être ouvert,
        // plutôt que d'exit le programme
        struct dirent *d;

        int total_size = 0;

        while ((d = readdir(dir)) != NULL) {
            char path[PATH_STORAGE_MAX];
            sprintf(path, "%s/%s", file_path, d->d_name);
            if (!is_dir(d->d_name) || is_dot_dir(d->d_name)) {
                total_size += get_size(path);
            } else {
                total_size += get_total_size(path);
            }
        }
        closedir(dir);
        return total_size;
    } else {
        return get_size(file_path);
    }
}

boolean is_filepath_a_file(char *filepath) {
    struct stat stats;
    if (stat(filepath, &stats) != 0) {
        return FALSE;
    }
    return S_ISREG(stats.st_mode) != 0 ? TRUE : FALSE;
}

int open_file(char filepath[MESSAGE_SIZE], short flag) {
    int fd = open(filepath, flag);
    if (fd == -1) {
        perror("Error");
        //printf("Error! Could not open file\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

boolean is_filepath_well_formed(char filepath[MESSAGE_SIZE]) {
    regex_t regex;
    if ((regcomp(&regex, "^((\\.{1,2}\\/)*|~\\/)?([A-Za-z0-9_]+\\/)+[A-Za-z0-9_]*(\\.[A-Za-z0-9_]+)?$",
                 REG_EXTENDED)) == 0) {
        int result = regexec(&regex, filepath, 0, NULL, 0);
        regfree(&regex);
        return result == 0 ? true : false;
    } else {
        printf("Problème interne sur le traitement du formattage\n");
    }
    return false;
}

char *get_file_name_from_filepath(char filepath[]) {
    char *p = strrchr(filepath, '/');
    return ++p;
}

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
