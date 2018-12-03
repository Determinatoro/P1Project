#ifndef P1PROJECT_HELPER_H
#define P1PROJECT_HELPER_H

/******************************/
/* STRUCTS */
/******************************/

/* TIME */
typedef struct {
    int hour;     /* Number of hours 24 format */
    int min;      /* Number of minutes */
    int sec;      /* Number of seconds */
} H_TIME;

/* DATE */
typedef struct {
    int day;      /* Day in month */
    int month;    /* Month of year */
    int year;     /* Year in yy format the number in the parenteses --> 20(18) */
} H_DATE;

/******************************/
/* PROTOTYPES */
/******************************/

void remove_cr_lf(char *line);
int starts_with_string(const char *str,const char *prefix);
int count_chars(const char *s, const char c);
int string_to_int_l(const char *str,const int str_length);
int string_to_int(const char *str);

#endif
