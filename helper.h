#ifndef P1PROJECT_HELPER_H
#define P1PROJECT_HELPER_H

/******************************/
/* PROTOTYPES */
/******************************/

void remove_cr_lf(char *line);
int starts_with_string(const char *str,const char *prefix);
int count_chars(const char *s, const char c);
int string_to_int_l(const char *str,const int str_length);
int string_to_int(const char *str);

#endif
