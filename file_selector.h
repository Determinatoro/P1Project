#include <dirent.h>

#ifndef P1PROJECT_FILE_SELECTOR_H
#define P1PROJECT_FILE_SELECTOR_H

typedef struct dirent DIRENT;

/******************************/
/* PROTOTYPES */
/******************************/

void print_directory_content(const DIRENT *dirent_arr,int number_of_dirents);
int get_directory_content(const char *dir_path, DIRENT **dirent_arr);
void test();

char *select_file();
int is_CSV_File(char *fileName);

#endif
