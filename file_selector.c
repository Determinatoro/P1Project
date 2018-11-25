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
char *select_file() {
    DIR *dir;
    dirent *file;
    dirent *allFiles;
    int counter = 0;
    int choice = 0;

    /* Open the directory and check if success or failure */
    dir = opendir(STANDARD_FOLDER);
    if(dir != NULL){
        /* Loop through our directory to figure out how many files there are */
        while(readdir(dir) != NULL)
            counter++;

        /* Rewind the directory to the first entry */
        rewinddir(dir);

        /* Allocate enough memory to store all of our files */
        allFiles = malloc(sizeof(dirent) * counter);

        /* If we managed to allocate enough memory we loop through the files again to store them */
        if(allFiles != NULL){
            int i = 0;
            while((file = readdir(dir)) != NULL){
                allFiles[i++] = *file;
            }
        }

        printf("Please select a file to use:\n");

        /* Print the names of the files in the directory */
        int i = 0;
        for (i = 0; i < counter; ++i) {
            if(is_CSV_File(allFiles[i].d_name) == 0)
                printf("File (%d): %s\n", i, allFiles[i].d_name);
        }

        while(choice >= counter || choice < 0 || is_CSV_File(allFiles[choice].d_name) != 0){
            printf("You need to specify a choice between 0-%d and only .csv are allowed!\n", counter);
            scanf("%d", &choice);
        }

        /* Close the directory after reading the files */
        closedir(dir);

        printf("File selected: %s\n", allFiles[choice].d_name);

        return allFiles[choice].d_name;
    }
    else{
        printf("The directory doesn't exist! Closing down..");
        return NULL;
    }
}

int is_CSV_File(char *fileName){
    /* Find the last '.' char in the filename to get the file extension */
    char *s = strrchr (fileName, '.');
    if(s != NULL)
        return strcmp(s, ".csv");
    else
        return -1;
}