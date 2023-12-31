#ifndef SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H
#define SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H

#include "../constants.h"
#include "../utils/bool_type.h"

#define STORING_PATH "../files_to_store/"

void create_file(char *filename);

int write_file_content(char *filename, char file_content[], size_t nb_char_to_write);

char *get_filename_from_message(char msg[INPUT_SIZE]);

boolean does_file_exist(char filename[]);

struct stat *get_struct_stat_of_file(char filename[]);

char *get_file_content_from_message(char msg[INPUT_SIZE]);

char *get_complete_filepath(char *filename);

#endif //SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_SERVER_H
