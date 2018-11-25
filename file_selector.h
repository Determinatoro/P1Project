#ifndef P1PROJECT_FILE_SELECTOR_H
#define P1PROJECT_FILE_SELECTOR_H

typedef struct dirent dirent;

/******************************/
/* PROTOTYPES */
/******************************/

char *select_file();
int is_CSV_File(char *fileName);

#endif //P1PROJECT_FILE_SELECTOR_H
