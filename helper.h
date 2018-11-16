//
// Created by japr on 15-11-2018.
//

#ifndef P1PROJECT_HELPER_H
#define P1PROJECT_HELPER_H

struct time {
    int hour;                               /* Number of hours 24 format */
    int min;                                /* Number of minutes */
    int sec;                                /* Number of seconds */
};
typedef struct time TIME;

struct date {
    int day;                                /* Day in month */
    int month;                              /* Month of year */
    int year;                               /* Year in yy format the number in the parenteses --> 20(18) */
};
typedef struct date DATE;

void remove_cr_lf(char *line,const int count);
int starts_with_string(const char *str,const char *prefix);
int string_to_int(const char *str,const int str_length);
int string_to_int_nl(const char *str);
int count_chars(char *s, char c);

#endif //P1PROJECT_HELPER_H
