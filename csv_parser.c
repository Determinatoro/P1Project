#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv_parser.h"
#include "file_helper.h"
#include "helper.h"

/******************************/
/* FUNCTIONS */
/******************************/

/* Split csv line with a given splitter */
int split_csv_line(char **column_values, char *line, char splitter) {
    int i, values_counter = 0, len = (int) strlen(line);
    char *value;

    value = line;
    for (i = 0; i <= len; i++) {
        char c = line[i];
        if (c == splitter || c == '\0') {
            line[i] = '\0';
            column_values[values_counter++] = value;
            value = line + i + 1;
        }
    }

    return values_counter;
}

/* Get the splitter used in the CSV file */
char get_csv_file_splitter(const char *line) {
    int number_of_kommas, number_of_semicolons;

    number_of_kommas = count_chars(line, ',');
    number_of_semicolons = count_chars(line, ';');

    return (char) ((number_of_kommas > number_of_semicolons) ? ',' : ';');
}

/***********************/
/* GPS LOGGER SPECIFIC */
/***********************/

/*
 * Split GPS Logger date time format into a date and time struct
 * Format example: 2018-11-16 17:00:00
 * */
void split_gps_logger_date_time(const char *date_time_str, DATE *date, TIME *time) {
    date->year = string_to_int_l(&date_time_str[2], 2);
    date->month = string_to_int_l(&date_time_str[5], 2);
    date->day = string_to_int_l(&date_time_str[8], 2);

    time->hour = string_to_int_l(&date_time_str[11], 2);
    time->min = string_to_int_l(&date_time_str[14], 2);
    time->sec = string_to_int_l(&date_time_str[17], 2);
}

/*
 * Parse GPS Logger packet
 * Format example: T,2018-11-16 09:20:56,50.12345678,4.12343444,4,70.182,,0.640,129,16,35,,
 */
int parse_gps_logger_packet(char *line, const char splitter, GPS_LOGGER_PACKET *gps_logger_packet) {
    char **column_values = malloc(strlen(line) + 1);
    int number_of_columns;

    number_of_columns = split_csv_line(column_values, line, splitter);
    if (number_of_columns == GPS_LOGGER_NUM_OF_COL) {
        split_gps_logger_date_time(column_values[1], &gps_logger_packet->date, &gps_logger_packet->time);

        gps_logger_packet->latitude = strtod(column_values[2], NULL);
        gps_logger_packet->longitude = strtod(column_values[3], NULL);
        gps_logger_packet->accuracy = string_to_int(column_values[4]);
        gps_logger_packet->altitude = strtod(column_values[5], NULL);

        gps_logger_packet->speed = strtod(column_values[7], NULL);
        /* Convert from m/s to km/h */
        gps_logger_packet->speed *= 3.6;

        gps_logger_packet->sat_used = string_to_int(column_values[9]);
        gps_logger_packet->sat_in_view = string_to_int(column_values[10]);
        return 1;
    }

    return 0;
}

/* Read GPS Logger file and get the packets */
int read_gps_logger_csv_file(const char *file_name, int *number_of_packets, GPS_LOGGER_PACKET *gps_logger_packets) {
    char **lines = NULL, *headers, splitter;
    int i, line_counter = 0;

    line_counter = get_number_of_lines_in_file(file_name);
    lines = malloc((size_t) (GPS_LOGGER_LINE_SIZE * line_counter));

    *number_of_packets = 0;


    if (read_file(file_name, &line_counter, GPS_LOGGER_LINE_SIZE, lines, "")) {
        headers = lines[0];
        splitter = get_csv_file_splitter(headers);
        lines = &lines[1];

        for (i = 0; i < line_counter - 1; i++) {
            GPS_LOGGER_PACKET gps_logger_packet;
            if (parse_gps_logger_packet(lines[i], splitter, &gps_logger_packet)) {
                gps_logger_packets[(*number_of_packets)++] = gps_logger_packet;
            }
        }

        free(lines);
        return 1;
    }

    return 0;
}
