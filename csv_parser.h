#ifndef P1PROJECT_CSV_PARSER_H
#define P1PROJECT_CSV_PARSER_H

#include "helper.h"

/******************************/
/* DEFINES */
/******************************/

#define GPS_LOGGER_LINE_SIZE    150
#define GPS_LOGGER_NUM_OF_COL   13

/******************************/
/* STRUCTS */
/******************************/

/* GPS LOGGER PACKET */
typedef struct {
    TIME time;          /* Time */
    DATE date;          /* Date */
    double latitude;    /* Latitude */
    double longitude;   /* Longitude */
    int accuracy;       /* Accuracy in meters */
    double altitude;    /* Altitude in meters */
    double speed;       /* Speed converted from m/s to km/h in the parser */
    int sat_used;       /* Satellites used */
    int sat_in_view;    /* Satellites in view */
} GPS_LOGGER_PACKET;

/******************************/
/* PROTOTYPES */
/******************************/

int split_csv_line(char **column_values, char *line, char splitter);
void split_gps_logger_date_time(const char *date_time_str,DATE *date,TIME *time);
int parse_gps_logger_packet(char *line,const char splitter,GPS_LOGGER_PACKET *gps_logger_packet);
char get_csv_file_splitter(const char *line);
int read_gps_logger_csv_file(const char *file_name,int *number_of_packets,GPS_LOGGER_PACKET *gps_logger_packets);

#endif //P1PROJECT_CSV_PARSER_H
