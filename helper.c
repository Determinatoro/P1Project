#include <string.h>
#include <math.h>
#include <ctype.h>
#include "helper.h"

/******************************/
/* FUNCTIONS */
/******************************/

/* Remove CR and LF from a string */
void remove_cr_lf(char *line, const int count) {
    if (*line == '\0')
        return;
    else if ((*line == '\r' || *line == '\n'))
        *line = '\0';
    remove_cr_lf(line + 1, count);
}

/* Find out if a string start with a given string */
int starts_with_string(const char *str, const char *prefix) {
    return (strlen(prefix) <= strlen(str))
           ? (*prefix != '\0' && *str == *prefix ? starts_with_string(str + 1, prefix + 1) : *prefix == '\0')
           : 0;
}

/* Count number of a given char in a string */
int count_chars(char *s, char c) {
    return *s == '\0'
           ? 0
           : count_chars(s + 1, c) + (*s == c);
}

/* Helper for the string to int functions */
int string_to_int_helper(const char *first, const char *last, const int str_length) {
    return (int) (last >= first && isdigit(*last)
                  ? (*last - '0') * pow(10, (int) (str_length - (last - first + 1))) +
                    string_to_int_helper(first, last - 1, str_length)
                  : 0);
}

/* Convert string to int with a given length */
int string_to_int_l(const char *str, const int str_length) {
    return string_to_int_helper(str, str + str_length - 1, str_length);
}

/* Convert string to int with no given length */
int string_to_int(const char *str) {
    return string_to_int_helper(str, str + strlen(str) - 1, (const int) strlen(str));
}