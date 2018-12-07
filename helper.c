#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

/******************************/
/* PROTOTYPES */
/******************************/
int string_to_int_helper(const char *first,const char *last,const int str_length);

/******************************/
/* FUNCTIONS */
/******************************/

/* Remove CR and LF from a string */
void remove_cr_lf(char *line) {
    char *last = line + strlen(line) - 1;
    if (*last == '\r' || *last == '\n') {
        *last = '\0';
        remove_cr_lf(line);
    }
}

/* Find out if a string start with a given string */
int starts_with_string(const char *str, const char *prefix) {
    return (strlen(prefix) <= strlen(str))
           ? (*prefix != '\0' && *str == *prefix ? starts_with_string(str + 1, prefix + 1) : *prefix == '\0')
           : 0;
}

/* Count number of a given char in a string */
int count_chars(const char *s, const char c) {
    return *s == '\0'
           ? 0
           : count_chars(s + 1, c) + (*s == c);
}

/* Check if string is a digit */
int is_string_a_digit(const char *first, const char *last) {
    return first == last ? 1 : isdigit(*first) && is_string_a_digit(first + 1, last);
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
    return !is_string_a_digit(str, str + str_length - 1) ? -1 : string_to_int_helper(str, str + str_length - 1,
                                                                                     str_length);
}

/* Convert string to int with no given length */
int string_to_int(const char *str) {
    return string_to_int_helper(str, str + strlen(str) - 1, (const int) strlen(str));
}

/* Clear the screen in the CLI */
void clear_screen() {
    int s = system("cls");
    if (s != 0)
        system("clear");
}