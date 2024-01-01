#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H

#include "../../constants.h"

void manage_request(char message[INPUT_SIZE]);

int start_server(int port);

int stop_server();

int get_message(char msg_read[INPUT_SIZE]);

void listen_message(char message[INPUT_SIZE]);

void list_files(char msg_to_send[INPUT_SIZE]);

#endif //SOFTSEC_PROJECT_SECTRANS_SERVER_MESSAGE_MANAGEMENT_H
