#ifndef SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H

void send_file(char filepath[], int port);
void download_file(char filename[], int port);
void login(char *username, char *passwd, int port);
void registration(char *username, char *passwd, int port);
void client_sendResponse(char action,int response,int port);
void sending(char msg_to_send[MESSAGE_SIZE], int port);
void add_action_at_index(char msg_to_send[MESSAGE_SIZE], char action, int index);

#endif //SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H
