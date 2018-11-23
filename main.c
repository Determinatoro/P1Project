#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gps_parser.h"
#include "csv_parser.h"
#include "file_helper.h"
#include "file_selector.h"

/*#define CSV_FILE "20181115-135922_jakob.csv"*/
#define CSV_FILE "20181116-102051.csv"

int main() {

    select_file();

    int i, input, number_of_packets, number_of_lines;
    GPS_LOGGER_PACKET *gps_logger_packets;

    number_of_lines = get_number_of_lines_in_file(CSV_FILE);
    gps_logger_packets = malloc(sizeof(GPS_LOGGER_PACKET) * number_of_lines);
    read_gps_logger_csv_file(CSV_FILE, &number_of_packets, gps_logger_packets);

    printf("\n");
    for (i = 0; i < number_of_packets; i++) {
        GPS_LOGGER_PACKET gps_logger_packet = gps_logger_packets[i];
        printf("%02d:%02d:%02d %10.7lf%10.7lf%3d%10.5lf\n", gps_logger_packet.time.hour, gps_logger_packet.time.min, gps_logger_packet.time.sec, gps_logger_packet.latitude, gps_logger_packet.longitude, gps_logger_packet.accuracy, gps_logger_packet.speed);
    }

    scanf("%d", &input);

    return 0;
}