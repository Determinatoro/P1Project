#ifndef P1PROJECT_FILE_HELPER_H
#define P1PROJECT_FILE_HELPER_H

#include <stdio.h>

/******************************/
/* PROTOTYPES */
/******************************/

FILE *open_file_w(const char *file_path);

int read_file(const char *file_name, int *number_of_lines, const int line_size, char ***lines, const char *filter);

#endif
