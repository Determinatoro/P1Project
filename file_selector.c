#include <stdio.h>
#include "file_selector.h"
#include "helper.h"
#include <string.h>
#include <malloc.h>
#include <unistd.h>

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
    char *p = getenv("USERNAME");
    char *standard_folder = malloc(sizeof(char) * (21 + strlen(p)));
    strcpy(standard_folder, "C:\\Users\\");
    strcpy(standard_folder + 9, p);
    strcpy(standard_folder + (9 + strlen(p)), "\\Documents");
    standard_folder[21 + strlen(p)] = '\0';

    open_new_folder(standard_folder);
}

void startUserDialog(DIRENT *dirent_arr, int number_of_dirents, char *current_folder){
    int choice = 1;
    char *new_folder;

    printf("Currently showing results for folder %s\n", current_folder);
    while(choice >= 1 && choice < (number_of_dirents + 2)){
        printf("Select a folder or file to continue:\n");
        printf("Provide folder index here -> ");
        scanf("%d", &choice);

        /* Go back */
        if(choice == 1){
            new_folder = go_back(current_folder);
            open_new_folder(new_folder);
        }
        else if(dirent_arr[choice - 2].d_type != 0) {
            new_folder = get_new_folder(current_folder, dirent_arr[choice - 2].d_name);
            open_new_folder(new_folder);
        } else {
            if(is_CSV_File(dirent_arr[choice - 2].d_name) == 0) {
                /* Select file */
                printf("File selected: %s\n", dirent_arr[choice - 2].d_name);
            } else{
                printf("The file selected is not a .CSV file! Please try again.\n");
            }
        }
    }
}

void open_new_folder(char *folder){
    DIRENT *dirent_arr;
    char *new_folder;
    int number_of_dirents;

    number_of_dirents = get_directory_content(folder, &dirent_arr);
    if(number_of_dirents > 0){
        print_directory_content(dirent_arr, number_of_dirents);
        startUserDialog(dirent_arr, number_of_dirents, folder);
    } else{
        printf("The folder you selected was empty! (%s)\n", folder);
        new_folder = go_back(folder);
        open_new_folder(new_folder);
    }
}

char *get_new_folder(char *current_folder, char *folder){
    char *new_folder = malloc(sizeof(char) * (strlen(current_folder) + strlen(folder) + 2));
    strcpy(new_folder, current_folder);
    strcpy(new_folder + strlen(current_folder), "\\");
    strcpy(new_folder + strlen(current_folder) + 1, folder);
    new_folder[(strlen(current_folder) + strlen(folder)) + 2] = '\0';

    return new_folder;
}

char *go_back(char *current_folder){
    char *new_folder;
    int i, length = strlen(current_folder);
    for(i = length; i > 0; i--){
        if(current_folder[i] == '\\'){
            new_folder = malloc(sizeof(char) * (i + 1));
            strncpy(new_folder, current_folder, (size_t)i);
            new_folder[i + 1] = '\0';
            break;
        }
    }

    return new_folder;
}

/* Lets the user choose a file from the specified directory */
char *select_file() {
    DIR *dir;
    DIRENT *dirent;
    DIRENT *dirent_array;
    int number_of_dirents = 0;
    int choice = 0;

    dir = opendir("");
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