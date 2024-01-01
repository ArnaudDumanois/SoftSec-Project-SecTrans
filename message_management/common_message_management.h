#ifndef SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H

#include "../constants.h"

void add_action(char msg_to_send[INPUT_SIZE], char action);
void add_filename(char msg_to_send[INPUT_SIZE], char filename[]);

#endif //SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
