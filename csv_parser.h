//
// Created by japr on 14-11-2018.
//

#ifndef P1PROJECT_CSV_PARSER_H
#define P1PROJECT_CSV_PARSER_H

#include "helper.h"

typedef struct {
    TIME time;
    DATE date;
    double latitude;
    double longitude;
    int accuracy;
    double altitude;
    double speed;
    int sat_used;
    int sat_in_view;
} GPS_LOGGER_PACKET;

int read_gps_logger_csv_file(const char *file_name, int *number_of_packets, GPS_LOGGER_PACKET *gps_logger_packets);

#endif //P1PROJECT_CSV_PARSER_H
