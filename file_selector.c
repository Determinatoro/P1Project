#include <stdio.h>
#include "file_selector.h"
#include "helper.h"
#include <string.h>
#include <malloc.h>

#define STANDARD_FOLDER "C:\\Users\\japr\\Documents"

/******************************/
/* FUNCTIONS */
/******************************/

void print_directory_content(const DIRENT *dirent_arr, int number_of_dirents) {
    int i;

    printf("%2d | %s\n", 1, "Go back");
    for (i = 0; i < number_of_dirents; i++) {
        printf("%2d | %s\n", i + 2, dirent_arr[i].d_name);
    }
}

int get_directory_content(const char *dir_path, DIRENT **dirent_arr) {
    DIR *dir;
    int number_of_dirents = 0;
    DIRENT *dirent;

    dir = opendir(dir_path);
    if (dir != NULL) {
        while(readdir(dir) != NULL)
            number_of_dirents++;

        rewinddir(dir);

        dirent_arr[0] = malloc(sizeof(DIRENT) * number_of_dirents);

        number_of_dirents = 0;

        while((dirent = readdir(dir)) != NULL){
            if (dirent->d_name[0] != '.')
                dirent_arr[0][number_of_dirents++] = *dirent;
        }

        closedir(dir);
        return number_of_dirents;
    }

    return 0;
}

void test() {
    DIRENT *dirent_arr;
    int number_of_dirents;

    number_of_dirents = get_directory_content(STANDARD_FOLDER, &dirent_arr);
    print_directory_content(dirent_arr, number_of_dirents);
}

/* Lets the user choose a file from the specified directory */
char *select_file() {
    DIR *dir;
    DIRENT *dirent;
    DIRENT *dirent_array;
    int number_of_dirents = 0;
    int choice = 0;

    dir = opendir(STANDARD_FOLDER);
    if(dir != NULL){

        while(readdir(dir) != NULL)
            number_of_dirents++;

        rewinddir(dir);

        dirent_array = malloc(sizeof(dirent) * number_of_dirents);

        if(dirent_array != NULL){
            int i = 0;
            while((dirent = readdir(dir)) != NULL){
                dirent_array[i++] = *dirent;
            }
        }

        printf("Please select a dirent to use:\n");

        /* Print the names of the files in the directory */
        int i = 0;
        for (i = 0; i < number_of_dirents; ++i) {
            if(is_CSV_File(dirent_array[i].d_name) == 0)
                printf("File (%d): %s\n", i, dirent_array[i].d_name);
        }

        while(choice >= number_of_dirents || choice < 0 || is_CSV_File(dirent_array[choice].d_name) != 0){
            printf("You need to specify a choice between 0-%d and only .csv are allowed!\n", number_of_dirents);
            scanf("%d", &choice);
        }

        /* Close the directory after reading the files */
        closedir(dir);

        printf("File selected: %s\n", dirent_array[choice].d_name);

        return dirent_array[choice].d_name;
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