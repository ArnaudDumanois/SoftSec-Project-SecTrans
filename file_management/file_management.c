#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "../util.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <regex.h>
#include "file_management.h"
#include <fcntl.h>
#include <string.h>


#define PATH_STORAGE_MAX 1024

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

int open_file(char filepath[INPUT_SIZE], short flag) {
    int fd = open(filepath, flag);
    if (fd == -1) {
        printf("Error! Could not open file\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

boolean is_filepath_well_formed(char filepath[INPUT_SIZE]) {
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

char* get_file_name_from_filepath(char filepath[INPUT_SIZE]){
    char *p = strrchr(filepath, '/');
    return ++p;
}



