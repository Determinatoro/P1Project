#include <stdio.h>
#include "file_selector.h"
#include <dirent.h>
#include <string.h>
#include <malloc.h>

#define STANDARD_FOLDER "C:\\Users\\andre\\Documents\\GPS_Files"

/******************************/
/* FUNCTIONS */
/******************************/

/* Lets the user choose a file from the specified directory */
void select_file() {
    DIR *dir;
    struct dirent *file;
    int counter = 0;

    /* Open the directory and check if success or failure */
    dir = opendir(STANDARD_FOLDER);
    if(dir != NULL){
        printf("Please select a file to use:\n");

        /* Print the names of the files in the directory */
        while((file = readdir(dir)) != NULL){
            printf("File (%d): %s\n", counter++, file->d_name);
        }

        /* Close the directory after reading the files */
        closedir(dir);
    }
    else{
        printf("The directory doesn't exist! Closing down..");
        return;
    }
}
