#ifndef P1PROJECT_ANALYZER_H
#define P1PROJECT_ANALYZER_H

#include <float.h>
#include "csv_parser.h"

#define WALK_SPEED_MIN      0
#define WALK_SPEED_MAX      10
#define BIKE_SPEED_MIN      10
#define BIKE_SPEED_MAX      25
#define CAR_SPEED_MIN       25
#define CAR_SPEED_MAX       500
#define PLANE_SPEED_MIN     500
#define PLANE_SPEED_MAX     DBL_MAX

enum Speed_Type {STOPPED, WALK, BIKE, CAR, PLANE};
typedef enum Speed_Type SPEED_TYPE;

typedef struct {
    SPEED_TYPE speed_type;
    GPS_LOGGER_PACKET *gps_logger_packets;
} ANALYZER_DATA;

int analyze_gps_logger_packet(const GPS_LOGGER_PACKET *gps_logger_packets,const int number_of_gps_logger_packets);

#endif
