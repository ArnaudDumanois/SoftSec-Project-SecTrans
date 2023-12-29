#ifndef SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H

#include "../constants.h"

int create_file(char *filename);

int write_file_content(int fd, char file_content[INPUT_SIZE]);

#endif //SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H
