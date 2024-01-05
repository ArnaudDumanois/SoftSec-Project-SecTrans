#include "common_message_management.h"
#include <string.h>

void add_action(char msg_to_send[INPUT_SIZE], char *action) {
    size_t lenAction = strlen(action);
    for(int i=0;i<lenAction;i++){msg_to_send[i]=action[i];}
    msg_to_send[lenAction]=';';
}

void add_filename(char msg_to_send[INPUT_SIZE], char filename[]) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, filename);
        msg_to_send[2 + strlen(filename)] = ';';
    }
}

void add_string(char msg_to_send[INPUT_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}

void add_string_without_dot(char msg_to_send[INPUT_SIZE], char *string) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, string);
        msg_to_send[2 + strlen(string)] = ';';
    }
}