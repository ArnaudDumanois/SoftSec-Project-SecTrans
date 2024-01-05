#ifndef SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H

void send_file(char filepath[], int port);
void download_file(char filename[], int port);
int login(char *username, char *passwd, int port);
void sending(char msg_to_send[INPUT_SIZE], int port);

#endif //SOFTSEC_PROJECT_SECTRANS_CLIENT_MESSAGE_MANAGEMENT_H
