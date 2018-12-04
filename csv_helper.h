#ifndef P1PROJECT_CSV_PARSER_H
#define P1PROJECT_CSV_PARSER_H

#include "helper.h"
#include <time.h>

/******************************/
/* PROTOTYPES */
/******************************/

int split_csv_line(char **column_values, char *line, char splitter);

char get_csv_file_splitter(const char *line);

#endif
