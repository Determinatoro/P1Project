#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gps_logger_helper.h"
#include "file_helper.h"
#include "csv_helper.h"

/******************************/
/* DEFINES */
/******************************/

/*#define DEBUG 1*/

/******************************/
/* PROTOTYPES */
/******************************/

void split_gps_logger_date_time(const char *date_time_str, TIME_DATA *time_data);

int get_speed_type(double speed);

double calculate_difference(TIME_DATA time_data1, TIME_DATA time_data2);

int get_start_block_index(const BLOCK_DATA *block_data_arr, int block_index);

int get_end_block_index(const BLOCK_DATA *block_data_arr, int block_data_arr_size, int block_index);

void add_gps_packets_to_tour(const BLOCK_DATA *block_data_arr, int start_tour_block_index, int end_tour_block_index,
                             int number_of_gps_packets, TOUR_DATA **tour_data_arr, int *tour_data_arr_size);

/***********************/
/* GPS LOGGER FILE FUNCTIONS */
/***********************/

/*
 * Split GPS Logger date time format into a date and time struct
 * Format example: 2018-11-16 17:00:00
 * */
void split_gps_logger_date_time(const char *date_time_str, TIME_DATA *time_data) {
    time_data->tm_year = string_to_int_l(&date_time_str[0], 4) - 1900;
    time_data->tm_mon = string_to_int_l(&date_time_str[5], 2);
    time_data->tm_mday = string_to_int_l(&date_time_str[8], 2);

    time_data->tm_hour = string_to_int_l(&date_time_str[11], 2);
    time_data->tm_min = string_to_int_l(&date_time_str[14], 2);
    time_data->tm_sec = string_to_int_l(&date_time_str[17], 2);
}

/*
 * Parse GPS Logger packet
 * Format example: T,2018-11-16 09:20:56,50.12345678,4.12343444,4,70.182,,0.640,129,16,35,,
 */
int parse_gps_logger_packet(char *line, const char splitter, GPS_LOGGER_PACKET *gps_logger_packet) {
    char **column_values = malloc(strlen(line) + 1);
    int number_of_columns;

    /* Split CSV line and get number of columns */
    number_of_columns = split_csv_line(column_values, line, splitter);
    if (number_of_columns == GPS_LOGGER_NUM_OF_COL) {
        /* Date and time */
        TIME_DATA time_data;
        memset(&time_data, 0, sizeof(TIME_DATA));
        split_gps_logger_date_time(column_values[1], &time_data);
        gps_logger_packet->time_data = time_data;
        /* Latitude */
        gps_logger_packet->latitude = strtod(column_values[2], NULL);
        /* Longitude */
        gps_logger_packet->longitude = strtod(column_values[3], NULL);
        /* Accuracy */
        gps_logger_packet->accuracy = string_to_int(column_values[4]);
        /* Altitude */
        gps_logger_packet->altitude = strtod(column_values[5], NULL);
        /* Speed */
        gps_logger_packet->speed = strtod(column_values[7], NULL);
        /* Convert from m/s to km/h */
        gps_logger_packet->speed *= 3.6;
        /* Satellites used */
        gps_logger_packet->sat_used = string_to_int(column_values[9]);
        /* Satellites in view */
        gps_logger_packet->sat_in_view = string_to_int(column_values[10]);
        return 1;
    }

    return 0;
}

/* Read GPS Logger file and parse the data lines into packets */
int read_gps_logger_csv_file(const char *file_path,
                             int *out_no_of_packets,
                             GPS_LOGGER_PACKET **out_gps_logger_packets) {
    char **lines = NULL, *headers, splitter;
    int i, line_counter = 0;

    /* Set start value */
    *out_no_of_packets = 0;

    /* Read file, get all the lines in the file back */
    if (!read_file(file_path, &line_counter, GPS_LOGGER_LINE_SIZE, &lines, ""))
        return 0;

    *out_gps_logger_packets = malloc(sizeof(GPS_LOGGER_PACKET) * line_counter);
    /* Get headers */
    headers = lines[0];
    splitter = get_csv_file_splitter(headers);

    /* Parse every line into a GPS Logger packet */
    for (i = 1; i < line_counter - 1; i++) {
        GPS_LOGGER_PACKET gps_logger_packet;
        if (parse_gps_logger_packet(lines[i], splitter, &gps_logger_packet)) {
            (*out_gps_logger_packets)[(*out_no_of_packets)++] = gps_logger_packet;
        }
    }

    free(lines);

    /* If there no valid data */
    if (*out_no_of_packets == 0)
        return 0;

    return 1;
}

/* Write data to CSV file */
int write_gps_logger_csv_file(const char *file_path, TOUR_DATA tour_data) {
    int i;
    FILE *fp;
    fp = open_file_w(file_path);
    if (fp == NULL) {
        return 0;
    }

    fprintf(fp, "type,time,latitude,longitude,accuracy,altitude,speed,sat_used,sat_inview\r\n");

    for (i = 0; i < tour_data.gps_logger_packets_size; i++) {
        fprintf(fp, "T,%04d-%02d-%02d %02d:%02d:%02d,%0.8f,%0.8f,%d,%0.3f,%0.3f,%d,%d\r\n",
                tour_data.gps_logger_packets[i].time_data.tm_year,
                tour_data.gps_logger_packets[i].time_data.tm_mon,
                tour_data.gps_logger_packets[i].time_data.tm_mday,
                tour_data.gps_logger_packets[i].time_data.tm_hour,
                tour_data.gps_logger_packets[i].time_data.tm_min,
                tour_data.gps_logger_packets[i].time_data.tm_sec,
                tour_data.gps_logger_packets[i].latitude,
                tour_data.gps_logger_packets[i].longitude,
                tour_data.gps_logger_packets[i].accuracy,
                tour_data.gps_logger_packets[i].altitude,
                tour_data.gps_logger_packets[i].speed / 3.6f,
                tour_data.gps_logger_packets[i].sat_used,
                tour_data.gps_logger_packets[i].sat_in_view);
    }

    fclose(fp);

    printf("Tour saved at %s\n", file_path);

    return 1;
}

/***********************/
/* GPS LOGGER ANALYZE FUNCTIONS */
/***********************/

/* Get speed type from the given speed */
int get_speed_type(double speed) {
    if (speed < 0.1) {
        return STOPPED;
    } else if (speed > WALK_SPEED_MIN && speed < WALK_SPEED_MAX_BIKE_SPEED_MIN) {
        return WALK;
    } else if (speed >= WALK_SPEED_MAX_BIKE_SPEED_MIN && speed < BIKE_SPEED_MAX_CAR_SPEED_MIN) {
        return BICYCLE;
    } else if (speed >= BIKE_SPEED_MAX_CAR_SPEED_MIN && speed < CAR_SPEED_MAX) {
        return CAR;
    } else {
        return PLANE;
    }
}

/* Get speed type as a string */
char *get_speed_type_str(SPEED_TYPE speed_type) {
    switch (speed_type) {
        case STOPPED:
            return "STOPPED";
        case WALK:
            return "WALK";
        case BICYCLE:
            return "BICYCLE";
        case CAR:
            return "CAR";
        case PLANE:
            return "PLANE";
    }
}

/* Get difference in time between 2 time data */
double calculate_difference(TIME_DATA time_data1, TIME_DATA time_data2) {
    time_t time1, time2;

    time1 = mktime(&time_data1);
    time2 = mktime(&time_data2);

    return difftime(time2, time1);
}

/* Get start block index for the tour */
int get_start_block_index(const BLOCK_DATA *block_data_arr, int block_index) {
    int j, index = block_index;
    /* Find out if there are any BIKE or CAR blocks before this one */
    for (j = block_index - 1; j >= 0; j--) {
        if ((block_data_arr[j].highest_speed_type == BICYCLE &&
             block_data_arr[j].average_speed > WALK_SPEED_MAX_BIKE_SPEED_MIN) ||
            block_data_arr[j].highest_speed_type == CAR)
            index = j;
        else
            break;
    }

    return index;
}

/* Get end block index for the tour */
int get_end_block_index(const BLOCK_DATA *block_data_arr, int block_data_arr_size, int block_index) {
    int j, index = block_index;
    /* Find out if there are any BIKE or CAR block after this one */
    for (j = block_index + 1; j < block_data_arr_size; j++) {
        if ((block_data_arr[j].highest_speed_type == BICYCLE &&
             block_data_arr[j].average_speed > WALK_SPEED_MAX_BIKE_SPEED_MIN) ||
            block_data_arr[j].highest_speed_type == CAR)
            index = j;
        else
            break;
    }

    return index;
}

/* Add GPS Logger packets to a tour */
void add_gps_packets_to_tour(const BLOCK_DATA *block_data_arr, int start_tour_block_index, int end_tour_block_index,
                             int number_of_gps_packets, TOUR_DATA **tour_data_arr, int *tour_data_arr_size) {
    int i, j;
    (*tour_data_arr)[*tour_data_arr_size].gps_logger_packets_size = 0;
    (*tour_data_arr)[*tour_data_arr_size].gps_logger_packets = malloc(
            sizeof(GPS_LOGGER_PACKET) * number_of_gps_packets);
    for (i = start_tour_block_index; i <= end_tour_block_index; i++) {
        for (j = 0; j < block_data_arr[i].gps_logger_packets_size; j++) {
            GPS_LOGGER_PACKET gps_logger_packet = block_data_arr[i].gps_logger_packets[j];
            (*tour_data_arr)[*tour_data_arr_size].gps_logger_packets[(*tour_data_arr)[*tour_data_arr_size].gps_logger_packets_size++] = gps_logger_packet;
        }
    }
    (*tour_data_arr_size)++;
}

/* Group the GPS logger packets in tours */
void group_gps_logger_packets_in_tours(const GPS_LOGGER_PACKET *gps_logger_packets,
                                       int number_of_gps_logger_packets,
                                       TOUR_DATA **out_tour_data_arr,
                                       int *out_tour_data_arr_size) {
    int i, j,
            block_data_arr_size = 0,
            block_grouping_started = 0,
            filtered_gps_logger_packets_size = 0,
            start_tour_block_index = -1,
            end_tour_block_index = -1;
    GPS_LOGGER_PACKET *filtered_gps_logger_packets;
    SPEED_TYPE speed_type;

    filtered_gps_logger_packets = malloc(sizeof(GPS_LOGGER_PACKET) * number_of_gps_logger_packets);

    /* Filter the GPS packets by looking at the accuracy */
    for (i = 0; i < number_of_gps_logger_packets; i++) {
        if (gps_logger_packets[i].accuracy <= HIGHEST_ACCURACY) {
            filtered_gps_logger_packets[filtered_gps_logger_packets_size] = gps_logger_packets[i];
            filtered_gps_logger_packets[filtered_gps_logger_packets_size++].speed_type = get_speed_type(
                    gps_logger_packets[i].speed);
        }
    }

    /* Allocate space for the block data */
    BLOCK_DATA *block_data_arr = malloc(sizeof(BLOCK_DATA) * filtered_gps_logger_packets_size);

    /* Group GPS Logger packets in blocks */
    for (i = 0; i < filtered_gps_logger_packets_size; i++) {
        speed_type = filtered_gps_logger_packets[i].speed_type;
        /* Start of new block */
        if (speed_type != STOPPED && !block_grouping_started) {
            /* Set flag */
            block_grouping_started = 1;

            BLOCK_DATA block_data;
            /* Set speed */
            block_data.total_speed = filtered_gps_logger_packets[i].speed;
            block_data.average_speed = block_data.total_speed;

            block_data.gps_logger_packets = malloc(sizeof(GPS_LOGGER_PACKET) * filtered_gps_logger_packets_size);
            block_data.gps_logger_packets_size = 0;
            block_data.gps_logger_packets[block_data.gps_logger_packets_size++] = filtered_gps_logger_packets[i];

            /* Set the blocks speed type */
            block_data.highest_speed_type = filtered_gps_logger_packets[i].speed_type;

            block_data_arr[block_data_arr_size++] = block_data;
        } /* Skip packets where the speed type is stopped */
        else if (speed_type == STOPPED) {
            block_grouping_started = 0;
        } /* If there has been added an block to the array */
        else if (block_data_arr_size > 0) {
            /* Add packet speed to the total speed */
            block_data_arr[block_data_arr_size - 1].total_speed += filtered_gps_logger_packets[i].speed;
            /* Add GPS Logger packet to the block */
            block_data_arr[block_data_arr_size - 1].
                    gps_logger_packets[block_data_arr[block_data_arr_size - 1].gps_logger_packets_size++] =
                    filtered_gps_logger_packets[i];
            /* Calculate average speed */
            block_data_arr[block_data_arr_size - 1].average_speed =
                    block_data_arr[block_data_arr_size - 1].total_speed /
                    block_data_arr[block_data_arr_size - 1].gps_logger_packets_size;
            /* Set speed type for the block */
            if (filtered_gps_logger_packets[i].speed_type >
                block_data_arr[block_data_arr_size - 1].highest_speed_type)
                block_data_arr[block_data_arr_size - 1].highest_speed_type =
                        filtered_gps_logger_packets[i].speed_type;
        }
    }

#ifdef DEBUG
    /* Write out the block data */

    for (i = 0; i < block_data_arr_size; i++) {
        printf("\n\nBLOCK %d\n\n", i);

        for (j = 0; j < block_data_arr[i].gps_logger_packets_size; j++) {
            printf("%5d %10s %0.7f,%0.7f  %02d:%02d:%02d %2.2f\n",
                   i,
                   get_speed_type_str(block_data_arr[i].gps_logger_packets[j].speed_type),
                   block_data_arr[i].gps_logger_packets[j].longitude, block_data_arr[i].gps_logger_packets[j].latitude,
                   block_data_arr[i].gps_logger_packets[j].time_data.tm_hour,
                   block_data_arr[i].gps_logger_packets[j].time_data.tm_min,
                   block_data_arr[i].gps_logger_packets[j].time_data.tm_sec,
                   block_data_arr[i].gps_logger_packets[j].speed);
        }
    }
#endif

    /* Allocate space for the tour data */
    *out_tour_data_arr = malloc(sizeof(TOUR_DATA) * block_data_arr_size);
    /* Set size of array to 0 */
    *out_tour_data_arr_size = 0;

    /* Group blocks in tours */
    for (i = 0; i < block_data_arr_size; i++) {
        /* If the blocks highest speed type is CAR and the average
         * speed is higher than the minimum car speed
         */
        if (block_data_arr[i].highest_speed_type == CAR &&
            block_data_arr[i].average_speed >= BIKE_SPEED_MAX_CAR_SPEED_MIN) {
            /* If there has already been found a block */
            if (start_tour_block_index != -1 && end_tour_block_index != -1) {
                /* Find BIKE and CAR block located before or after the current block */
                int start_block_index = get_start_block_index(block_data_arr, i);
                int end_block_index = get_end_block_index(block_data_arr, block_data_arr_size, i);

                /*
                 * Find the difference in time between the last
                 * GPS LOGGER packet in the last block and the first GPS
                 * LOGGER packet in this block
                 */
                double diff = abs(
                        (int) calculate_difference(
                                block_data_arr[start_block_index].gps_logger_packets[0].time_data,
                                block_data_arr[end_tour_block_index].gps_logger_packets[
                                        block_data_arr[end_tour_block_index].gps_logger_packets_size -
                                        1].time_data));

                /* If the difference is lower than the set value set new end index */
                if (diff <= HIGHEST_DIFF_SEC) {
                    end_tour_block_index = end_block_index;
                    i = end_tour_block_index;
                } else {
                    /* Add GPS LOGGER packets to tour data */
                    add_gps_packets_to_tour(block_data_arr,
                                            start_tour_block_index,
                                            end_tour_block_index,
                                            filtered_gps_logger_packets_size,
                                            out_tour_data_arr,
                                            out_tour_data_arr_size);

                    /* Set indedes for new tour */
                    start_tour_block_index = start_block_index;
                    end_tour_block_index = end_block_index;
                    i = end_tour_block_index;
                }
            } else {
                /* Find BIKE and CAR block located before or after the current block */
                start_tour_block_index = get_start_block_index(block_data_arr, i);
                end_tour_block_index = get_end_block_index(block_data_arr, block_data_arr_size, i);

                /* Set i to the end so the blocks are not gone through twice */
                i = end_tour_block_index;
            }
        }
    }

    if (start_tour_block_index != -1 && end_tour_block_index != -1) {
        /* Add GPS LOGGER packets to tour data */
        add_gps_packets_to_tour(block_data_arr,
                                start_tour_block_index,
                                end_tour_block_index,
                                filtered_gps_logger_packets_size,
                                out_tour_data_arr,
                                out_tour_data_arr_size);
    }


    free(filtered_gps_logger_packets);
    free(block_data_arr);
}