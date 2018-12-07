#include <dirent.h>

#ifndef P1PROJECT_FILE_SELECTOR_H
#define P1PROJECT_FILE_SELECTOR_H

/******************************/
/* DEFINES */
/******************************/

#define DOCUMENTS_FOLDER    "C:\\Users\\%s\\Documents"

/******************************/
/* STRUCTS */
/******************************/

typedef struct dirent DIRENT;

/******************************/
/* PROTOTYPES */
/******************************/

char *start_user_dialog(char **folder);
int is_exit_cmd(char *str);

#endif
