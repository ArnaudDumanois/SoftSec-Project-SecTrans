#ifndef SOFTSEC_PROJECT_SECTRANS_LOAD_LIBRARIES_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_LOAD_LIBRARIES_SERVER_H

void loadLibrary_server();
void unloadLibrary_server();
void call_start_server(int port);
void call_stop_server();
void call_get_msg_server(char message[1024]);

#endif //SOFTSEC_PROJECT_SECTRANS_LOAD_LIBRARIES_SERVER_H
