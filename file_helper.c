#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_helper.h"
#include "helper.h"

/******************************/
/* PROTOTYPES */
/******************************/

int get_number_of_lines_in_file_helper(FILE *file);
FILE *open_file_r(int *number_of_lines, char *file_name);
int get_number_of_lines_in_file(const char *file_name);

/******************************/
/* FUNCTIONS */
/******************************/

/* Count lines in file */
int get_number_of_lines_in_file_helper(FILE *file) {
    int lines = 1, ch;
    if (file != NULL) {
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n')
                lines++;
        }

        rewind(file);

        return lines;
    }

    return 0;
}

/* Open file and count the lines */
int get_number_of_lines_in_file(const char *file_name) {
    int lines;
    FILE *fp;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        lines = get_number_of_lines_in_file_helper(fp);
        fclose(fp);
        return lines;
    }
    return 0;
}

/* Open a file and count the lines */
FILE *open_file_r(int *number_of_lines, char *file_name) {
    FILE *fp;
    *number_of_lines = 0;
    fp = fopen(file_name, "r");
    if (fp != NULL) {
        *number_of_lines = get_number_of_lines_in_file_helper(fp);
    }

    return fp;
}

/* Read file */
int read_file(const char *file_name, int *number_of_lines, const int line_size, char ***lines, const char *filter) {
    int line_counter = 0;
    FILE *fp;
    char *str = NULL, *line = NULL;

    *number_of_lines = 0;

    if ((fp = open_file_r(&line_counter, (char *) file_name)) == NULL) {
        printf("\nCould not open file: %s\n", file_name);
        return 0;
    }

    lines[0] = malloc((size_t) (line_size * line_counter));
    str = malloc((size_t) line_size);

    while (fgets(str, line_size, fp) != NULL) {
        line = malloc(strlen(str));
        strcpy(line, str);
        remove_cr_lf(line);

        if (strcmp(line, "") == 0)
            continue;
        else if (strcmp(filter, "") == 0) {
            lines[0][(*number_of_lines)++] = line;
        } else {
            if (starts_with_string(line, filter)) {
                lines[0][(*number_of_lines)++] = line;
            }
        }
    }
    fclose(fp);

    return 1;
}
