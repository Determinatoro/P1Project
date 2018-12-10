#include <stdio.h>
#include "file_selector.h"
#include "helper.h"
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

/******************************/
/* PROTOTYPES */
/******************************/

int is_csv_file(char *fileName);

void print_directory_content(const char *current_folder, const DIRENT *dirent_arr, int number_of_dirents);

int get_directory_content(const char *dir_path, DIRENT **dirent_arr);

char *get_new_folder(char *current_folder, char *folder);

char *go_back(char *current_folder);

void open_new_folder(DIRENT **dirent_arr, int *number_of_dirents, char *folder);

int is_at_documents(const char *folder);

/******************************/
/* FUNCTIONS */
/******************************/

void print_directory_content(const char *current_folder, const DIRENT *dirent_arr, int number_of_dirents) {
    int i, start_index = 1;
    printf("%2d | %s\n", start_index++, "Go back");
    if (number_of_dirents > 0) {
        for (i = 0; i < number_of_dirents; i++) {
            printf("%2d | %s\n", i + start_index, dirent_arr[i].d_name);
        }
    } else
        printf("FOLDER IS EMPTY!\n");
}

/* Get the directory content back in a DIRENT array */
int get_directory_content(const char *dir_path, DIRENT **dirent_arr) {
    DIR *dir;
    int number_of_dirents = 0;
    DIRENT *dirent;

    dir = opendir(dir_path);
    if (dir != NULL) {
        /* Get number of files and folders in the directory */
        while (readdir(dir) != NULL)
            number_of_dirents++;

        /* Reset pointer */
        rewinddir(dir);

        *dirent_arr = malloc(sizeof(DIRENT) * number_of_dirents);

        number_of_dirents = 0;

        while ((dirent = readdir(dir)) != NULL) {
            /* Filter hidden files and folders away */
            if (dirent->d_name[0] != '.') {
                /* Only show CSV files and folders */
                if(((dirent->d_type == 0 || dirent->d_type == 8) && is_csv_file(dirent->d_name)) ||
                    (dirent->d_type != 0 && dirent->d_type != 8)){
                    (*dirent_arr)[number_of_dirents++] = *dirent;
                }
            }
        }

        closedir(dir);
        return number_of_dirents;
    }

    return 0;
}

char *start_user_dialog(char **folder) {
    DIRENT *dirent_arr;
    int choice = 1, res = 0, number_of_dirents = 0;
    char *file_selected, *current_folder;
    char *p = getenv("USERNAME");
    char choiceStr[5];

    current_folder = malloc(sizeof(char) * sizeof(DOCUMENTS_FOLDER) + strlen(p));
    sprintf(current_folder, DOCUMENTS_FOLDER, p);

    open_new_folder(&dirent_arr, &number_of_dirents, current_folder);

    while (1) {
        printf("Use 'exit' to terminate the program.\nSelect index -> ");
        res = scanf(" %4s", choiceStr);

        if (res == 1) {
            if(is_exit_cmd(choiceStr)){
                printf("Terminating program!");
                return "";
            }

            res = sscanf(choiceStr, "%d", &choice);
            if(res == 1){
                if (choice > 0 && (choice - 2) < number_of_dirents) {
                    if (choice == 1) {
                        if (!is_at_documents(current_folder)) {
                            current_folder = go_back(current_folder);
                            open_new_folder(&dirent_arr, &number_of_dirents, current_folder);
                        } else
                            printf("You can't go further back!\n");
                    } else if (dirent_arr[choice - 2].d_type != 0 && dirent_arr[choice - 2].d_type != 8) {
                        current_folder = get_new_folder(current_folder, dirent_arr[choice - 2].d_name);
                        open_new_folder(&dirent_arr, &number_of_dirents, current_folder);
                    } else {
                        if (is_csv_file(dirent_arr[choice - 2].d_name)) {
                            /* Select file */
                            file_selected = calloc(sizeof(char),
                                                   (strlen(current_folder) + strlen(dirent_arr[choice - 2].d_name) + 2));
                            strcpy(file_selected, current_folder);
                            strcpy(file_selected + strlen(current_folder), "\\");
                            strcpy(file_selected + strlen(current_folder) + 1, dirent_arr[choice - 2].d_name);
                            file_selected[(strlen(current_folder) + strlen(dirent_arr[choice - 2].d_name) + 1)] = '\0';
                            *folder = malloc(sizeof(char) * strlen(current_folder));
                            *folder = current_folder;
                            clear_screen();
                            printf("File selected: %s\n", file_selected);
                            return file_selected;
                        } else {
                            printf("The file selected is not a .CSV file! Please try again.\n");
                        }
                    }
                }
            }
        }
    }
}

void open_new_folder(DIRENT **dirent_arr, int *number_of_dirents, char *folder) {
    clear_screen();
    *number_of_dirents = get_directory_content(folder, &(*dirent_arr));
    printf("Showing results for folder %s\n", folder);
    print_directory_content(folder, *dirent_arr, *number_of_dirents);
}

char *get_new_folder(char *current_folder, char *folder) {
    char *new_folder = calloc(sizeof(char), (strlen(current_folder) + strlen(folder) + 2));
    strcpy(new_folder, current_folder);
    strcpy(new_folder + strlen(current_folder), "\\");
    strcpy(new_folder + strlen(current_folder) + 1, folder);
    new_folder[(strlen(current_folder) + strlen(folder)) + 1] = '\0';
    return new_folder;
}

char *go_back(char *current_folder) {
    char *new_folder = "";
    int i, length = (int) strlen(current_folder);
    for (i = length; i > 0; i--) {
        if (current_folder[i] == '\\') {
            new_folder = calloc(sizeof(char), (size_t) (i) + 1);
            current_folder[i] = '\0';
            strcpy(new_folder, current_folder);
            break;
        }
    }

    return new_folder;
}

int is_csv_file(char *fileName) {
    /* Find the last '.' char in the filename to get the file extension */
    char *s = strrchr(fileName, '.');
    if (s != NULL)
        return strcmp(s, ".csv") == 0;
    else
        return 0;
}

int is_at_documents(const char *folder) {
    int i, length = (int) strlen(folder);
    for (i = length; i > 0; i--) {
        if (folder[i] == '\\') {
            return strncmp(folder + (i + 1), "Documents", 9) == 0;
        }
    }
    return 0;
}

int is_exit_cmd(char *str){
    int i = 0;
    for(i = 0; i < strlen(str); i++){
        str[i] = (char)tolower(str[i]);
    }

    return strcmp(str, "exit") == 0;
}