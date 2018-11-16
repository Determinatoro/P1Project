#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_helper.h"
#include "helper.h"

/* Open a file and count the lines */
FILE *open_file(int *number_of_lines, char *file_name) {
    FILE *fp;
    *number_of_lines = get_number_of_lines_in_file(file_name);
    fp = fopen(file_name, "r");
    return fp;
}

/* Count lines in a file */
int get_number_of_lines_in_file(const char *file_name) {
    int ch = 0, lines = 0;
    FILE *fp;

    fp = fopen(file_name, "r");

    if (fp == NULL) {
        return 0;
    }

    lines++;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
            lines++;
    }

    return lines;
}



/* Read file */
int read_file(const char *file_name, int *number_of_lines, const int line_size, char **lines, const char *filter) {
    int line_counter = 0;
    FILE *fp;
    char *str = NULL, *line = NULL;

    *number_of_lines = 0;

    fp = open_file(&line_counter, (char *) file_name);
    if (fp == NULL) {
        printf("\nCould not open file %s\n", file_name);
        return 0;
    }

    str = malloc((size_t) line_size);

    while (fgets(str, line_size, fp) != NULL) {
        line = malloc(strlen(str));
        strcpy(line, str);
        remove_cr_lf(line, (int) strlen(line));

        if (strcmp(line, "") == 0)
            continue;
        else if (strcmp(filter, "") == 0) {
            lines[(*number_of_lines)++] = line;
        } else {
            if (starts_with_string(line, filter)) {
                lines[(*number_of_lines)++] = line;
            }
        }
    }
    fclose(fp);

    return 1;
}
