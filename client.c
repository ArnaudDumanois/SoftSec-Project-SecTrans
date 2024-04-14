#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "file_management/file_management.h"
#include "server.h"
#include "message_management/client_message_management/client_message_management.h"
#include "message_management/server_message_management/server_message_management.h"
#include "./libs/client_libs/messages.h"
#include "libs/logger/logger_sha.h"

int main(int argc, char *argv[]) {
    start_server(CLIENT_PORT);
    printf("Serveur démarré sur le port %d\n", CLIENT_PORT);

    if (strncmp(argv[0], "./client", 8) == 0) {
            if (strncmp(argv[1], "-list", 5) == 0) {
            // call server to get all files that this user deployed
        }
            else if (strncmp(argv[1], "-up", 3) == 0) {
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
                           MESSAGE_SIZE - 1);
                }
            }
            else if (strncmp(argv[1], "-down", 5) == 0) {
                // check that argv[2] is a path correctly formed, etc
                unsigned int size = strlen(argv[2]);
                if (size < MAX_FILEPATH_SIZE) {
                    start_server(CLIENT_PORT);
                    printf("Serveur côté client démarré sur le port %d\n", CLIENT_PORT);
                    // Check that argv[2] is the name of a file and not a filepath
                    download_file(argv[2], SERVER_PORT);
                    stop_server();
                } else {
                    printf("The path to your file is of size : %u. It should be of a maximum size of : %d", size,
                           MESSAGE_SIZE - 1);
                }
            }
            else if (strncmp(argv[1], "-login", 6) == 0){
                if(argc!=4){
                    printf("%s", "Wrong command format, See manual bellow !\n");
                    printf("%s",getManualForSpecificCommand(argv[1]));
                }
                else{
                    login(argv[2],argv[3],2000);
                    char message[MESSAGE_SIZE];
                    int res = listen_message(message);
                    if(res==AUTH_DONE){printf("LOGIN SUCCESFULL");}
                    else{printf(("LOGIN ERROR !"));}
                }
            }
            else if (strncmp(argv[1], "-register",9) == 0){
                if(argc!=4){
                    printf("%s", "Wrong command format, See manual bellow !\n");
                    printf("%s",getManualForSpecificCommand(argv[1]));
                }
                else{
                    registration(argv[2],argv[3],2000);
                    char message[MESSAGE_SIZE];
                    int res = listen_message(message);
                    if(res==REGISTRATION_DONE){printf("USER REGISTERED !");}
                    else if(res==USER_ALREADY_EXIST){printf("Un compte existe déjà");}
                    else if(res==ERROR_PASSWORD_TOO_LONG){ printf("PASSWORD TOO LONG !\n");}
                    else if(res==ERROR_USERNAME_TOO_LONG){printf("USERNAME TOO LONG !\n");}
                    else if(res==ERROR_BAD_USERNAME_INPUT){printf("USERNAME BAD INPUT !\n");}
                    else{printf(("REGISTRATION ERROR !"));}
                }
            }
    }
    else printf("Three commands are available : ./client -up filepath ; ./client -down filename ; ./client -list");

    printf("\n");
    return -1;
}