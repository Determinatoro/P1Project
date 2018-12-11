#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>

#include "gps_logger_helper.h"
#include "file_helper.h"
#include "file_selector.h"

int main() {
    int i, number_of_packets, tour_data_arr_size;
    GPS_LOGGER_PACKET *gps_logger_packets;
    TOUR_DATA *tour_data_arr;
    char *folder = NULL;

    /* Get path for the selected file */
    char *file = start_user_dialog(&folder);
    /* Read the file and get the packets */
    if (!read_gps_logger_csv_file(file, &number_of_packets, &gps_logger_packets)) {
        exit(0);
    }

    /*for (i = 0; i < number_of_packets; i++) {
        GPS_LOGGER_PACKET gps_logger_packet = gps_logger_packets[i];
        printf("%02d:%02d:%02d %10.7lf%10.7lf%3d%10.5lf\n", gps_logger_packet.time.hour, gps_logger_packet.time.min, gps_logger_packet.time.sec, gps_logger_packet.latitude, gps_logger_packet.longitude, gps_logger_packet.accuracy, gps_logger_packet.speed);
    }*/

    /* Split the packets into tours */
    group_gps_logger_packets_in_tours(gps_logger_packets, number_of_packets, &tour_data_arr, &tour_data_arr_size);

    printf("\nAnalyze finished!\n");

    /* Print the tours */
    for (i = 0; i < tour_data_arr_size; i++) {
        char *file_name = calloc(sizeof(char), strlen(folder) + 11);
        strcpy(file_name, folder);
        strcpy(file_name + strlen(folder), "\\");
        sprintf(file_name + strlen(folder) + 1, "tour%d.csv", i + 1);
        file_name[strlen(folder) + 10] = '\0';
        write_gps_logger_csv_file(file_name, tour_data_arr[i]);
    }

    /* Free the memory */
    free(tour_data_arr);
    free(gps_logger_packets);

    printf("\nPress Any Key to Continue...");
    _getch();

    return 0;
}