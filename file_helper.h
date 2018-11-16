//
// Created by japr on 14-11-2018.
//

#include <stdio.h>
#ifndef P1PROJECT_FILE_HELPER_H
#define P1PROJECT_FILE_HELPER_H

FILE *open_file(int *number_of_lines,char *file_name);
int get_number_of_lines_in_file(const char *file_name);
int read_file(const char *file_name,int *number_of_lines,const int line_size,char **lines,const char *filter);


#endif //P1PROJECT_FILE_HELPER_H
