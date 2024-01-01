#include "common_message_management.h"
#include <string.h>

void add_action(char msg_to_send[INPUT_SIZE], char action) {
    msg_to_send[0] = action;
    msg_to_send[1] = ';';
}

void add_filename(char msg_to_send[INPUT_SIZE], char filename[]) { // TODO : checks to do to not exceed array's size
    if(msg_to_send[1] == ';'){
        strcat(msg_to_send, filename);
        msg_to_send[2 + strlen(filename)] = ';';
    }
}