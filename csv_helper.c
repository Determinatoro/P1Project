#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_helper.h"
#include "file_helper.h"
#include "helper.h"

/******************************/
/* FUNCTIONS */
/******************************/

/* Split csv line with a given splitter */
int split_csv_line(char **column_values, char *line, char splitter) {
    int i, values_counter = 0, len = (int) strlen(line);
    char *value;

    value = line;
    for (i = 0; i <= len; i++) {
        char c = line[i];
        if (c == splitter || c == '\0') {
            line[i] = '\0';
            column_values[values_counter++] = value;
            value = line + i + 1;
        }
    }

    return values_counter;
}

/* Get the splitter used in the CSV file */
char get_csv_file_splitter(const char *line) {
    return (char) ((count_chars(line, ',') > count_chars(line, ';')) ? ',' : ';');
}
