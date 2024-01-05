#ifndef SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H

#include "../constants.h"

void add_action(char msg_to_send[INPUT_SIZE], char action);
void add_filename(char msg_to_send[INPUT_SIZE], char filename[]);

void add_string(char msg_to_send[INPUT_SIZE], char *string);

void add_string_without_dot(char msg_to_send[INPUT_SIZE], char *string);

#endif //SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
