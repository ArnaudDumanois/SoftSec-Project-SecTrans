#ifndef SOFTSEC_PROJECT_SECTRANS_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_SERVER_H

int startserver(int port);
int stopserver();

/* read message sent by client */
int getmsg(char msg_read[1024]);

#endif
