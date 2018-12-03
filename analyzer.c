#include <stdio.h>
#include <limits.h>

#include "analyzer.h"

int analyze_gps_logger_packet(const GPS_LOGGER_PACKET *gps_logger_packets, const int number_of_gps_logger_packets) {
    int i;

    for (i = 0; i < number_of_gps_logger_packets; i++) {
        printf("%2.2f Accuracy: %2d\n", gps_logger_packets[i].speed, gps_logger_packets[i].accuracy);
    }
}