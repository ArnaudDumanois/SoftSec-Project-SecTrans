#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_H

int start_server(int port);
int stop_server();

/* read message sent by client */
int get_message(char msg_read[1024]);

#endif
