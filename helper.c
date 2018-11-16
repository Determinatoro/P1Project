#include <string.h>
#include <math.h>
#include <ctype.h>
#include "helper.h"

void remove_cr_lf(char *line, const int count) {
    int i = 0;
    for (i = 0; i < count; i++) {
        char c = line[i];
        if (c == '\r' || c == '\n')
            line[i] = '\0';
    }
}

int starts_with_string(const char *str, const char *prefix) {
    const char *temp = str;
    const char *temp2 = prefix;

    if (strlen(prefix) <= strlen(str)) {
        while (*temp2 != '\0' && *temp == *temp2) {
            temp++;
            temp2++;
        }

        return *temp2 == '\0';
    }

    return 0;
}

int count_chars(char *s, char c) {
    return *s == '\0'
           ? 0
           : count_chars(s + 1, c) + (*s == c);
}

int string_to_int(const char *str, const int str_length) {
    int number = 0, counter = 0;
    const char *temp = str + str_length - 1;
    while (temp >= str && isdigit(*temp)) {
        number += (*temp - '0') * pow(10, counter++);
        temp -= 1;
    }

    return number;
}

int string_to_int_nl(const char *str) {
    int number = 0, counter = 0;
    const char *temp = str + strlen(str) - 1;
    while (temp >= str && isdigit(*temp)) {
        number += (*temp - '0') * pow(10, counter++);
        temp -= 1;
    }

    return number;
}