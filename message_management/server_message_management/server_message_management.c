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
#include <regex.h>
#include "../common_message_management.h"
#include "../../libs/logger/logger_sha.h"
#include "../../actions.h"

void list_files(char msg_to_send[MESSAGE_SIZE]) { // TODO : to finish
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

void manage_file(const char action, char filename[],
                 char file_content_to_write[]) { // TODO : checks when write returns 0 byte written, and send stop to client
    if (is_action(action,ACTION_CREATE) || is_action(action,ACTION_ADD)) {
        printf("ACTION_CREATE\n");
        //int nb_bytes_wrote;
        //printf("DOES FILES EXIST : %d\n", does_file_exist(filename));
        if (does_file_exist(filename) == FALSE) {
            printf("filename to create : %s\n", filename);
            create_file(filename);
        }
        write_file_content(filename, file_content_to_write, strlen(file_content_to_write));
        //printf("nb bytes written : %d\n", nb_bytes_wrote);
    } else if (is_action(action,ACTION_END)) {
        printf("The file has been created !\n");
    }
}


void sendingResponse(char action,int res){
    printf("SERVER RESPONSE\n");
    client_sendResponse(action,res,CLIENT_PORT);
}

boolean is_correctusername(char *usrname) {
    regex_t regex;
    if ((regcomp(&regex, "^[0-9a-zA-Z]+$",REG_EXTENDED)) == 0) {
        int result = regexec(&regex, usrname, 0, NULL, 0);
        regfree(&regex);
        return result == 0 ? TRUE : FALSE;
    } else {
        printf("Problème interne sur le traitement du formattage\n");
    }
    return FALSE;
}

int manage_request(char message[MESSAGE_SIZE]) {
    printf("MANAGE REQUEST\n");
    char action = message[0];

    if (is_action(action,ACTION_CREATE) || is_action(action,ACTION_ADD) ||
    is_action(action,ACTION_END)) {
        char *filename = malloc(total_size_between_semicolons(message,1)+1);
        extract_between_semicolons_at_index(message,1,filename, sizeof(filename));

        char *file_content = malloc(total_size_between_semicolons(message,2)+1);
        extract_between_semicolons_at_index(message,2,file_content, sizeof(file_content));
        //printf("Message content : %s\n", file_content);
        manage_file(action, filename, file_content);
        free(filename);
        clear_array(message, MESSAGE_SIZE);
    }
    else if (is_action(action,ACTION_DOWNLOAD)) {
        char *filename = malloc(total_size_between_semicolons(message,1)+1);
        extract_between_semicolons_at_index(message,1,filename, sizeof(filename));
        char *filepath = get_complete_filepath_storing(filename);
        printf("%s\n", filepath);
        send_file(filepath, CLIENT_PORT);
        free(filename);
        free(filepath);
        clear_array(message, MESSAGE_SIZE);
    }
    else if (is_action(action,ACTION_LOGIN)) {
        printf("LOGIN ACTION !\n");
        int res;
        size_t username_size = total_size_between_semicolons(message,1);
        if(username_size>(size_t)MAX_USERNAME_LENGTH){res=ERROR_USERNAME_TOO_LONG;printf("USERNAME TOO LONG !\n");}
        else{
            char *usrname = calloc(username_size+1, sizeof(char));
            extract_between_semicolons_at_index(message,1,usrname,username_size);
            if(is_correctusername(usrname)==FALSE){res=ERROR_BAD_USERNAME_INPUT;printf("BAD USERNAME\n");free(usrname);}
            else{
                size_t passwd_size = total_size_between_semicolons(message,2);
                if(passwd_size>(size_t)MAX_PASSWORD_LENGTH){res=ERROR_PASSWORD_TOO_LONG;printf("PASSWORD TOO LONG !\n");free(usrname);}
                else{
                    char *passwd = calloc(passwd_size+1, sizeof(char));
                    extract_between_semicolons_at_index(message,2,passwd, passwd_size);
                    res = authenticate_user(usrname,passwd);
                    free(passwd);
                    free(usrname);
                    if (res == AUTH_DONE) { printf("User login !\n"); }
                    else { printf("Error during user login !\n"); }
                }
            }
        }
        sendingResponse(action,res);
    }
    else if (is_action(action,ACTION_REGISTER)){
        printf("REGISTER ACTION !\n");
        int res;
        size_t username_size = total_size_between_semicolons(message,1);
        if(username_size>(size_t)MAX_USERNAME_LENGTH){res = ERROR_USERNAME_TOO_LONG;printf("USERNAME TOO LONG !\n");}
        else{
            char *usrname = calloc(username_size+1, sizeof(char));
            extract_between_semicolons_at_index(message,1,usrname, username_size);
            if(is_correctusername(usrname)==FALSE){res=ERROR_BAD_USERNAME_INPUT;printf("BAD USERNAME\n");free(usrname);}
            else{
                size_t passwd_size = total_size_between_semicolons(message,2);
                if(passwd_size>(size_t)MAX_PASSWORD_LENGTH){res=ERROR_PASSWORD_TOO_LONG;printf("PASSWORD TOO LONG !\n");free(usrname);}
                else{
                    char *passwd = calloc(passwd_size+1, sizeof(char));
                    extract_between_semicolons_at_index(message,2,passwd, passwd_size);
                    res = save_user(usrname,passwd);
                    free(passwd);
                    free(usrname);
                    if (res == 1) { printf("User registered !\n"); }
                    else { printf("Error during user registered !\n"); }
                }
            }
        }
        sendingResponse(action,res);
    }
    else if (is_action(action,ACTION_REPONSE)) {
        printf("REPONSE RECEIVED !");
        char *res = malloc(total_size_between_semicolons(message, 2));
        extract_between_semicolons_at_index(message, 2, res, sizeof(res));
        return (int) strtol(res, NULL, 10);
    }
    return -99;
}

int listen_message(char message[MESSAGE_SIZE]) {
        get_message(message);
        printf("Message reçu : %s\n", message);
        return manage_request(message);
}