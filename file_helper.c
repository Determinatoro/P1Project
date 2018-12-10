#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_helper.h"
#include "helper.h"

/******************************/
/* PROTOTYPES */
/******************************/

int get_number_of_lines_in_file(FILE *file);

FILE *open_file_r(int *number_of_lines, char *file_path);

/******************************/
/* FUNCTIONS */
/******************************/

/* Count lines in file */
int get_number_of_lines_in_file(FILE *file) {
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

/* OPen to write to a file */
FILE *open_file_w(const char *file_path) {
    FILE *fp;
    fp = fopen(file_path, "w");
    return fp;
}

/* Open a file and count the lines */
FILE *open_file_r(int *number_of_lines, char *file_path) {
    FILE *fp;
    *number_of_lines = 0;
    fp = fopen(file_path, "r");
    if (fp != NULL) {
        *number_of_lines = get_number_of_lines_in_file(fp);
    }

    return fp;
}

/* Read file */
int read_file(const char *file_name, int *number_of_lines, const int line_size, char ***lines, const char *filter) {
    int line_counter = 0;
    FILE *fp;
    char *str = NULL, *line = NULL;

    *number_of_lines = 0;

    /* Open file with read privileges and get number of lines */
    if ((fp = open_file_r(&line_counter, (char *) file_name)) == NULL) {
        printf("\nCould not open file: %s\n", file_name);
        return 0;
    }

    /* Allocate space for the lines */
    *lines = malloc((size_t) (line_size * line_counter));
    str = malloc(sizeof(char) * line_size);

    /* Read each line */
    while (fgets(str, line_size, fp) != NULL) {
        /* Copy the line content */
        line = malloc(strlen(str));
        strcpy(line, str);
        /* Remove CR and LF from the line */
        remove_cr_lf(line);

        /* If the line is empty skip it */
        if (strcmp(line, "") == 0)
            continue;
            /* If the filter is set to nothing */
        else if (strcmp(filter, "") == 0) {
            (*lines)[(*number_of_lines)++] = line;
        } /* If the line starts with the filter */
        else {
            if (starts_with_string(line, filter)) {
                (*lines)[(*number_of_lines)++] = line;
            }
        }
    }
    /* Close file */
    fclose(fp);

    return 1;
}
