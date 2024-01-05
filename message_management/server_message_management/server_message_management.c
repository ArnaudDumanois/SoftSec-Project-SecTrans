#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../file_management/file_management.h"
#include "../../file_management/file_management_server.h"
#include "../../utils/array_utils.h"
#include "../client_message_management/client_message_management.h"
#include "../../server.h"
#include <dirent.h>
#include <unistd.h>
#include "../common_message_management.h"
#include "../../libs/logger/logger_sha.h"

void list_files(char msg_to_send[INPUT_SIZE]) { // TODO : to finish
    DIR *d;
    struct dirent *dir;
    d = opendir(STORING_PATH);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            add_filename(msg_to_send, dir->d_name);
        }
        closedir(d);
    }
}

void manage_file(const char *action, char filename[],
                 char file_content_to_write[]) { // TODO : checks when write returns 0 byte written, and send stop to client
    if (strcmp(action,ACTION_CREATE)==0 || strcmp(action,ACTION_ADD)==0) {
        printf("ACTION_CREATE\n");
        //int nb_bytes_wrote;
        //printf("DOES FILES EXIST : %d\n", does_file_exist(filename));
        if (does_file_exist(filename) == FALSE) {
            printf("filename to create : %s\n", filename);
            create_file(filename);
        }
        write_file_content(filename, file_content_to_write, strlen(file_content_to_write));
        //printf("nb bytes written : %d\n", nb_bytes_wrote);
    } else if (strcmp(action,ACTION_END)==0) {
        printf("The file has been created !\n");
    }
}


void sendingResponse(char *action,int res){
    printf("SERVER RESPONSE\n");
    client_sendResponse(action,res,CLIENT_PORT);
}

int manage_request(char message[INPUT_SIZE]) {
    printf("MANAGE REQUEST\n");
    char *action = malloc(total_size_between_semicolons(message,0));
    extract_between_semicolons_at_index(message,0,action, sizeof(action));

    if (strcmp(action,ACTION_CREATE)==0 || strcmp(action,ACTION_ADD)==0 || strcmp(action,ACTION_END)==0) {
        char *filename = malloc(total_size_between_semicolons(message,1));
        extract_between_semicolons_at_index(message,1,filename, sizeof(filename));

        char *file_content = malloc(total_size_between_semicolons(message,2));
        extract_between_semicolons_at_index(message,2,file_content, sizeof(file_content));
        //printf("Message content : %s\n", file_content);
        manage_file(action, filename, file_content);
        free(filename);
        clear_array(message, INPUT_SIZE);
    }
    else if (strcmp(action,ACTION_DOWNLOAD)==0) {
        char *filename = malloc(total_size_between_semicolons(message,1));
        extract_between_semicolons_at_index(message,1,filename, sizeof(filename));
        char *filepath = get_complete_filepath_storing(filename);
        printf("%s\n", filepath);
        send_file(filepath, CLIENT_PORT);
        free(filename);
        free(filepath);
        clear_array(message, INPUT_SIZE);
    }
    else if (strcmp(action,ACTION_LOGIN)==0) {
        printf("LOGIN ACTION !\n");
        //vérifier si user existe
        char *usrname = malloc(total_size_between_semicolons(message,1));
        extract_between_semicolons_at_index(message,1,usrname,sizeof(usrname));

        char *passwd = malloc(total_size_between_semicolons(message,2));
        extract_between_semicolons_at_index(message,2,passwd, sizeof(passwd));

        int res = authenticate_user(usrname,passwd);

        if (res == 1) { printf("Auth Réussie"); }
        else { printf("Auth échouée"); }
    }
    else if (strcmp(action,ACTION_REGISTER)==0){
        printf("REGISTER ACTION !\n");
        char *usrname = malloc(total_size_between_semicolons(message,1));
        extract_between_semicolons_at_index(message,1,usrname,sizeof(usrname));

        char *passwd = malloc(total_size_between_semicolons(message,2));
        extract_between_semicolons_at_index(message,2,passwd, sizeof(passwd));

        int res = save_user(usrname,passwd);
        if (res == 1) { printf("User registered !\n"); }
        else { printf("Error during user registered !\n"); }

        sendingResponse(action,res);
    }
    else if (strcmp(action,ACTION_REPONSE)==0) {
        printf("REPONSE RECEIVED !");
        char *res = malloc(total_size_between_semicolons(message, 2));
        extract_between_semicolons_at_index(message, 2, res, sizeof(res));
        return (int) strtol(res, NULL, 10);
    }
    return -99;
}

int listen_message(char message[INPUT_SIZE]) {
        get_message(message);
        printf("Message reçu : %s\n", message);
        return manage_request(message);
}