#ifndef SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_H
#define SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_H
#include "../bool_type.h"
#include "../constants.h"

int get_size(char *file_path);
int get_total_size(char *file_path);
boolean is_filepath_well_formed(char filepath[INPUT_SIZE]);
int open_file(char filepath[INPUT_SIZE], short flag);
char* get_file_name_from_filepath(char filepath[INPUT_SIZE]);

#endif //SOFTSEC_PROJECT_SECTRANS_FILE_MANAGEMENT_H
