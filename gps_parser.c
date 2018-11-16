#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "gps_parser.h"
#include "file_helper.h"
#include "helper.h"

#define MAX_CHAR 1000

int split_nmea_packet(char **values, char *packet) {
    int i, values_counter = 0, len = (int) strlen(packet);
    char *value;

    value = packet;
    for (i = 0; i <= len; i++) {
        char c = packet[i];
        if (c == ',' || c == '\0' || c == '*') {
            packet[i] = '\0';
            values[values_counter++] = value;
            value = packet + i + 1;
        }
    }

    return values_counter;
}

void parse_nmea_time(TIME *time, const char *str) {
    time->hour = string_to_int_l(&str[0], 2);
    time->min = string_to_int_l(&str[2], 2);
    time->sec = string_to_int_l(&str[4], 2);
}

void parse_nmea_date(DATE *date, const char *str) {
    date->day = string_to_int_l(&str[0], 2);
    date->month = string_to_int_l(&str[2], 2);
    date->year = string_to_int_l(&str[4], 2);
}

void parse_latitude_longitude(float *value, char *unit, const char *str, const char *unit_str) {
    float degrees, minutes;
    *value = str[0] == '\0' ? 0.0f : strtof(str, NULL);
    *unit = unit_str[0];
    *value = (*unit == 'S' || *unit == 'W') ? *value *= -1.0 : *value;
    degrees = truncf(*value / 100.0f);
    minutes = *value - (degrees * 100.0f);
    *value = degrees + minutes / 60.0f;
}

int parse_gps_packet(GPS_PACKET *gps_packet, const char *packet) {
    int i, j, res,
            values_counter;
    char dummy,
            **temp_arr = NULL,
            **values = NULL,
            *temp_packet = calloc(sizeof(char), MAX_CHAR);

    res = sscanf(packet, "%c%[^,],%[^*]*%d", &dummy, gps_packet->Type, temp_packet, &gps_packet->checksum);
    if (res == 4) {

        values = calloc(sizeof(char), MAX_CHAR);

        if (strcmp(gps_packet->Type, GPGGA) == 0) {
            values_counter = split_nmea_packet(values, temp_packet);

            if (values_counter == 14) {
                GPGGA_PACKET gpgga;

                /* Time */
                TIME time;
                parse_nmea_time(&time, values[0]);
                gpgga.time = time;
                /* Latitude */
                parse_latitude_longitude(&gpgga.latitude, &gpgga.latitude_direction, values[1], values[2]);
                /* Longitude */
                parse_latitude_longitude(&gpgga.longitude, &gpgga.longitude_direction, values[3], values[4]);
                /* Fix quality */
                gpgga.fix_quality = (int) strtof(values[5], NULL);
                /* Number of satellites */
                gpgga.number_of_satellites = (int) strtof(values[6], NULL);
                /* Horizontal dil. of position */
                gpgga.hor_dil_of_position = strtof(values[7], NULL);
                /* Altitude */
                gpgga.altitude = strtof(values[8], NULL);
                /* Altitude unit */
                gpgga.altitude_unit = values[9][0];
                /* Height of geo ID */
                gpgga.height_of_geo_id = strtof(values[10], NULL);
                /* Height of geo ID unit */
                gpgga.height_of_geo_id_unit = values[11][0];

                gps_packet->gpgga_packet = gpgga;
            }
        } else if (strcmp(gps_packet->Type, GPRMC) == 0) {
            values_counter = split_nmea_packet(values, temp_packet);

            if (values_counter == 12 || values_counter == 11) {
                GPRMC_PACKET gprmc;

                /* Time */
                TIME time;
                parse_nmea_time(&time, values[0]);
                gprmc.time = time;
                /* A = Active V = Void */
                gprmc.active = values[1][0];
                /* Latitude */
                parse_latitude_longitude(&gprmc.latitude, &gprmc.latitude_direction, values[2], values[3]);
                /* Longitude */
                parse_latitude_longitude(&gprmc.longitude, &gprmc.longitude_direction, values[4], values[5]);
                /* Speed */
                gprmc.speed = strtof(values[6], NULL);
                /* Track angle in degrees */
                gprmc.track_angle = strtof(values[7], NULL);
                /* Date */
                DATE date;
                parse_nmea_date(&date, values[8]);
                gprmc.date = date;
                /* Magnetic variation */
                gprmc.magnetic_variation = strtof(values[9], NULL);
                /* Magnetic variation unit */
                gprmc.magnetic_variation_unit = values[10][0];

                gps_packet->gprmc_packet = gprmc;
            }
        } else if (strcmp(gps_packet->Type, GPGSV) == 0) {
            values_counter = split_nmea_packet(values, temp_packet);
            if (values_counter <= 19) {
                GPGSV_PACKET gpgsv;

                gpgsv.number_of_sentences = string_to_int_l(values[0], (int) strlen(values[0]));
                gpgsv.sentence_number = string_to_int_l(values[1], (int) strlen(values[1]));
                gpgsv.satellites_in_view = string_to_int_l(values[2], (int) strlen(values[2]));

                temp_arr = &values[3];
                gpgsv.satellites = malloc(sizeof(GPGSV_SATELLITE) * ((values_counter - 3) / 4));
                int satellites_counter = 0;

                for (i = 0; i < ((values_counter - 3) / 4); i++) {
                    GPGSV_SATELLITE satellite;
                    for (j = 0; j < ((i * 4) % 4) + 4; j++) {
                        int index = j;
                        int *temp_ptr = (int *) &satellite;
                        temp_ptr = temp_ptr + (2 * index);
                        *temp_ptr = string_to_int_l(temp_arr[index], (int) strlen(temp_arr[index]));
                    }
                    gpgsv.satellites[satellites_counter++] = satellite;
                }

                gps_packet->gpgsv_packet = gpgsv;
            }
        }

        free(temp_packet);
        free(values);
    }

    return 1;
}

void read_nmea_file(const char *filename, GPS_PACKET *gps_packets) {
    char **lines;
    int i, line_counter = 0, gps_packets_size = 0;

    line_counter = get_number_of_lines_in_file(filename);
    lines = malloc((size_t) (MAX_CHAR * line_counter));

    if (read_file(filename, &line_counter, MAX_CHAR, lines, "$")) {
        gps_packets = malloc(sizeof(GPS_PACKET) * line_counter);

        for (i = 0; i < line_counter; i++) {
            GPS_PACKET gps_packet;
            parse_gps_packet(&gps_packet, lines[i]);
            gps_packets[gps_packets_size++] = gps_packet;
        }

        free(gps_packets);
    }
    free(lines);
}