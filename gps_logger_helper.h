#ifndef P1PROJECT_ANALYZER_H
#define P1PROJECT_ANALYZER_H

#include <float.h>
#include "csv_helper.h"

/******************************/
/* DEFINES */
/******************************/

#define HIGHEST_ACCURACY                    25
#define HIGHEST_DIFF_SEC                    240
#define GPS_LOGGER_LINE_SIZE                150
#define GPS_LOGGER_NUM_OF_COL               13

#define WALK_SPEED_MIN                      0
#define WALK_SPEED_MAX_BIKE_SPEED_MIN       6.0f
#define BIKE_SPEED_MAX_CAR_SPEED_MIN        17.0f
#define CAR_SPEED_MAX                       500

/******************************/
/* ENUMS AND STRUCTS */
/******************************/

/* SPEED TYPE enum */
typedef enum {
    STOPPED, WALK, BICYCLE, CAR, PLANE
} SPEED_TYPE;

/* TIME DATA struct */
typedef struct tm TIME_DATA;

/* GPS LOGGER PACKET struct */
typedef struct {
    TIME_DATA time_data;
    double latitude;    /* Latitude */
    double longitude;   /* Longitude */
    int accuracy;       /* Accuracy in meters */
    double altitude;    /* Altitude in meters */
    double speed;       /* Speed converted from m/s to km/h in the parser */
    int sat_used;       /* Satellites used */
    int sat_in_view;    /* Satellites in view */
    SPEED_TYPE speed_type;
} GPS_LOGGER_PACKET;

/* BLOCK DATA struct */
typedef struct {
    SPEED_TYPE highest_speed_type;
    int gps_logger_packets_size;
    GPS_LOGGER_PACKET *gps_logger_packets;
    double total_speed;
    double average_speed;
} BLOCK_DATA;

/* TOUR DATA struct */
typedef struct {
    int gps_logger_packets_size;
    GPS_LOGGER_PACKET *gps_logger_packets;
} TOUR_DATA;

/******************************/
/* PROTOTYPES */
/******************************/

int parse_gps_logger_packet(char *line, const char splitter, GPS_LOGGER_PACKET *gps_logger_packet);

int read_gps_logger_csv_file(const char *file_path, int *out_no_of_packets, GPS_LOGGER_PACKET **out_gps_logger_packets);

int write_gps_logger_csv_file(const char *file_path, TOUR_DATA tour_data);

void group_gps_logger_packets_in_tours(const GPS_LOGGER_PACKET *gps_logger_packets, int number_of_gps_logger_packets,
                                       TOUR_DATA **out_tour_data_arr, int *out_tour_data_arr_size);

#endif
