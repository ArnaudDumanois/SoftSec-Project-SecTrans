#include "client.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_management/file_management.h"
#include <unistd.h>

static void (*snd_msg)(char msg[INPUT_SIZE], int port);

int send_file_name(char filepath[], int port);


static void *libraryHandle;

void loadLibrary_client() {
    libraryHandle = dlopen("../dynlib/libclient.so", RTLD_LAZY);
    if (!libraryHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    snd_msg = dlsym(libraryHandle, "sndmsg");
    if (dlerror() != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unloadLibrary_client() {
    dlclose(libraryHandle);
}

int get_next_blocks(int fd, char file_content[INPUT_SIZE], int nb_bytes_to_read) {
    int nb_bytes_read;
    nb_bytes_read = read(fd, file_content, nb_bytes_to_read);
    if (nb_bytes_read == -1) {
        perror("Error! Could not read file\n");
        exit(EXIT_FAILURE);
    }
    return nb_bytes_read;
}

void clear_array(char array[], int nb_elements_to_clear) {
    for (int i = 0; i < nb_elements_to_clear; i++) {
        array[i] = '\0';
    }
}

boolean get_current_msg_to_send(int fd, char msg_to_send[INPUT_SIZE], char filepath[], char action) {
    char *filename = get_file_name_from_filepath(filepath);

    msg_to_send[0] = action;
    msg_to_send[1] = ';';
    strcat(&msg_to_send[2], filename);
    size_t filename_length = strlen(filename);
    msg_to_send[2 + filename_length] = ';'; // the filename begins at index 2

    int nb_bytes_max_to_read = INPUT_SIZE - filename_length - 3; // for \0 at the end and the two ; in the middle

    int nb_bytes_read = get_next_blocks(fd, &msg_to_send[2 + filename_length + 1], nb_bytes_max_to_read);
    if (nb_bytes_read == nb_bytes_max_to_read) {
        return FALSE; // is all file read ?
    } else return TRUE; // end of file or error -> stop fetching next data
}

boolean sending_common(int fd, char msg_to_send[INPUT_SIZE], char filepath[], char action, int port) {
    boolean result = get_current_msg_to_send(fd, msg_to_send, filepath, action);
    if (sndmsg(msg_to_send, port) != 0) {
        printf("Server can't receive your file - Port error");
        exit(EXIT_FAILURE);
    }
    return result;
}

boolean first_send(int fd, char msg_to_send[INPUT_SIZE], char filepath[], int port) {
    return sending_common(fd, msg_to_send, filepath, 'S', port);
}

boolean mid_send(int fd, char msg_to_send[INPUT_SIZE], char filepath[], int port) {
    return sending_common(fd, msg_to_send, filepath, 'A', port);
}

boolean final_send(int fd, char msg_to_send[INPUT_SIZE], char filepath[], int port) {
    return sending_common(fd, msg_to_send, filepath, 'E', port);
}

void send_file(char filepath[], int port) {
    int fd = open_file(filepath, 0); // 0 = read only
    char msg_to_send[INPUT_SIZE] = {'\0'};
    boolean result = first_send(fd, msg_to_send, filepath, port);
    clear_array(msg_to_send, INPUT_SIZE);
    while (result == FALSE) {
        result = mid_send(fd, msg_to_send, filepath, port);
        clear_array(msg_to_send, INPUT_SIZE);
    }
    close(fd);
    printf("Message sent !\n");
}

int sndmsg(char msg[INPUT_SIZE], int port) {
    if (port != SERVER_PORT) return -1;
    snd_msg(msg, port);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2 || argc == 3) {
        printf("%d\n", strncmp(argv[0], "./client", 8));
        if (strncmp(argv[0], "./client", 8) == 0) {
            loadLibrary_client();
            if (argc == 2) {
                if (strncmp(argv[1], "-list", 5) == 0) {
                    // call server to get all files that this user deployed
                }
            } else {
                if (strncmp(argv[1], "-up", 3) == 0) {
                    // check that argv[2] is a path correctly formed, etc
                    unsigned int size = strlen(argv[2]);
                    if (size < MAX_FILEPATH_SIZE) {
                        if (is_filepath_well_formed(argv[2]) == TRUE) {
                            send_file(argv[2], SERVER_PORT);
                        } else {
                            printf("The path to your file is wrong-formed");
                        }
                    } else {
                        printf("The path to your file is of size : %u. It should be of a maximum size of : %d", size,
                               INPUT_SIZE - 1);
                    }
                } else if (strncmp(argv[1], "-down", 5) == 0) {
                    // check that argv[2] is a path correctly formed, etc
                }
            }
        }
    } else printf("Three commands are available : ./client -up filepath ; ./client -down filename ; ./client -list");
    return -1;
}