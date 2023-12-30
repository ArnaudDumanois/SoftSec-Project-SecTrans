#include "client.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_management/file_management.h"
#include <unistd.h>
#include "file_management/io/io_client.h"

#define STILL 1
#define END 0

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

int get_next_blocks_file(char *current_pointer_to_file_content, char msg_to_send[], int nb_max_bytes_to_read) {
    int i = 0;
    while (i < nb_max_bytes_to_read) {
        if (current_pointer_to_file_content == NULL) {
            return END;
        }
        msg_to_send[i] = *current_pointer_to_file_content;
        i++;
        current_pointer_to_file_content++;
    }
    return current_pointer_to_file_content + 1 == NULL ? END : STILL;
}

void clear_array(char array[], int nb_elements_to_clear) {
    for (int i = 0; i < nb_elements_to_clear; i++) {
        array[i] = '\0';
    }
}

int
get_current_msg_to_send(char *ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], char action) {
    msg_to_send[0] = action;
    msg_to_send[1] = ';';
    strcat(&msg_to_send[2], filename);
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
sending_common(char *ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], char action, int port) {
    int result = get_current_msg_to_send(ptr_current_file_content, msg_to_send, filename, action);
    sending(msg_to_send, port);
    return result;
}

int first_send(char *ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], int port) {
    return sending_common(ptr_current_file_content, msg_to_send, filename, 'S', port);
}

int mid_send(char *ptr_current_file_content, char msg_to_send[INPUT_SIZE], char filename[], int port) {
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
    char *file_content = read_entire_file(fd);
    close(fd);

    char msg_to_send[INPUT_SIZE] = {'\0'};
    char *filename = get_file_name_from_filepath(filepath);
    int result = first_send(file_content, msg_to_send, filename, port);
    clear_array(msg_to_send, INPUT_SIZE);
    while (result == END) {
        result = mid_send(file_content, msg_to_send, filename, port);
        clear_array(msg_to_send, INPUT_SIZE);
    }
    final_send(msg_to_send, filename, port);
    clear_array(msg_to_send, INPUT_SIZE);
    printf("Message sent !\n");
}

int sndmsg(char msg[INPUT_SIZE], int port) {
    if (port != SERVER_PORT) return -1;
    snd_msg(msg, port);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2 || argc == 3) {
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