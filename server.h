//
// Created by arnau on 20/12/2023.
//

#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_H

int startserver(int port);
int stopserver();

/* read message sent by client */
int getmsg(char msg_read[1024]);
#endif //SOFTSEC_PROJECT_SECTRANS_SERVER_H
