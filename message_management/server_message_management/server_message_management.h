#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H

#include "../../constants.h"

int manage_request(char message[MESSAGE_SIZE]);

int listen_message(char message[MESSAGE_SIZE]);

void list_files(char msg_to_send[MESSAGE_SIZE]);

void sendingResponse(char *action,int res);

#endif //SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H
