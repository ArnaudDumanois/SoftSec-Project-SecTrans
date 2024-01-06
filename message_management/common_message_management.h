#ifndef SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H

#include <stddef.h>
#include "../constants.h"

void add_action(char msg_to_send[MESSAGE_SIZE], char action);
void add_filename(char msg_to_send[MESSAGE_SIZE], char filename[]);
void extract_between_semicolons_at_index(const char *input, int index, char *output, size_t output_size);
size_t total_size_between_semicolons(const char *input, int start_index);

void add_string(char msg_to_send[INPUT_SIZE], char *string);

void add_string_without_dot(char msg_to_send[INPUT_SIZE], char *string);

void insert_between_semicolons(char *input, const char new_action);
int is_valid_action(const char action);
#endif //SOFTSEC_PROJECT_SECTRANS_COMMON_MESSAGE_MANAGEMENT_H
