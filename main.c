#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>

#include "analyzer.h"
#include "gps_parser.h"
#include "file_helper.h"
#include "file_selector.h"

/*#define CSV_FILE "20181115-135922_jakob.csv"*/
#define CSV_FILE "20181116-102051.csv"


int main() {
    test();

    //select_file();
    int i, input, number_of_packets;
    GPS_LOGGER_PACKET *gps_logger_packets;

    return EXIT_SUCCESS;
    read_gps_logger_csv_file(CSV_FILE, &number_of_packets, &gps_logger_packets);
    printf("\n");
    /*for (i = 0; i < number_of_packets; i++) {
        GPS_LOGGER_PACKET gps_logger_packet = gps_logger_packets[i];
        printf("%02d:%02d:%02d %10.7lf%10.7lf%3d%10.5lf\n", gps_logger_packet.time.hour, gps_logger_packet.time.min, gps_logger_packet.time.sec, gps_logger_packet.latitude, gps_logger_packet.longitude, gps_logger_packet.accuracy, gps_logger_packet.speed);
    }*/

    analyze_gps_logger_packet(gps_logger_packets, number_of_packets);

    free(gps_logger_packets);

    scanf("%d", &input);

    return 0;
}