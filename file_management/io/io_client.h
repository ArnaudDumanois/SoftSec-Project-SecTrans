#ifndef SOFTSEC_PROJECT_SECTRANS_IO_CLIENT_H
#define SOFTSEC_PROJECT_SECTRANS_IO_CLIENT_H

#include <stdio.h>

//char *read_entire_file(int fd);
int readall(int fd, char **dataptr, size_t *sizeptr);

#endif //SOFTSEC_PROJECT_SECTRANS_IO_CLIENT_H
