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
int is_dir_a_folder(char *fileName);
void startUserDialog(DIRENT *dirent_arr, int number_of_dirents, char *current_folder);
char *get_new_folder(char *current_folder, char *folder);
char *go_back(char *current_folder);
void open_new_folder(char *folder);
int is_at_documents(char *folder);

#endif
