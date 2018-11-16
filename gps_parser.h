//
// Created by japr on 22-10-2018.
//

#ifndef P1PROJECT_GPS_DATA_PARSER_H
#define P1PROJECT_GPS_DATA_PARSER_H

#include "helper.h"

#define GPGGA "GPGGA"
#define GPRMC "GPRMC"
#define GPGSA "GPGSA"
#define GPGSV "GPGSV"

typedef struct {
    TIME time;                              /* Fix time */
    float latitude;                         /* Latitude */
    char latitude_direction;                /* Latitude direction */
    float longitude;                        /* Longitude */
    char longitude_direction;               /* Longitude direction */
    int fix_quality;                        /* Fix quality:
                                             * 0 = Invalid
                                             * 1 = GPS fix (SPS)
                                             * 2 = DGPS fix
                                             * 3 = PPS fix
                                             * 4 = Real Time Kinematic
                                             * 5 = Float RTK
                                             * 6 = Estimated (Dead reckoning) (2.3 feature)
                                             * 7 = Manual Input mode
                                             * 8 = Simulation mode
                                             */
    int number_of_satellites;               /* Number of satellites being tracked */
    float hor_dil_of_position;              /* Horizontal dilution of position */
    float altitude;                         /* Altitude, Meters, above mean sea level */
    char altitude_unit;                     /* Altitude unit */
    float height_of_geo_id;                 /* Height of geoid (Mean sea level) above WGS84 */
    char height_of_geo_id_unit;             /* Height of geoid unit */
} GPGGA_PACKET;

typedef struct {
    TIME time;                              /* Fix time */
    DATE date;                              /* Fix date */
    char active;                            /* Status A = Active or V = Void */
    float latitude;                         /* Latitude */
    char latitude_direction;                /* Latitude direction */
    float longitude;                        /* Longitude */
    char longitude_direction;               /* Longitude direction */
    float speed;                            /* Speed over the ground in knots */
    float track_angle;                      /* Track angle in degrees true */
    float magnetic_variation;               /* Magnetic variation */
    char magnetic_variation_unit;           /* Magnetic variation unit */
} GPRMC_PACKET;


typedef struct {
    char auto_selection;                    /* Auto selection of 2D or 3D fix (M = manual) */
    char fix_status;                        /* 1 = no fix, 2 = 2D fix, 3 = 3D fix */
    char prns_of_satellites[12][2];         /* PRNs of satellites used for fix (Space for 12) */
    float pdop;                             /* Dilution of precision */
    float horizontal_dil_of_precision;      /* Horizontal dilution of precision */
    float vertical_dil_of_precision;        /* Vertical dilution of precision */
} GPGSA_PACKET;

typedef struct {
    int satellite_prn_number;               /* Satellite PRN number */
    int elevation;                          /* Elevation in degrees */
    int azimuth;                            /* Azimuth in degrees */
    int snr;                                /* SNR - higher is better */
} GPGSV_SATELLITE;

typedef struct gpgsv_packet {
    int number_of_sentences;                /* Number of sentences for full data */
    int sentence_number;                    /* Sentence number */
    int satellites_in_view;                 /* Number of satellites in view */
    GPGSV_SATELLITE *satellites;            /* Satellite data */
} GPGSV_PACKET;

typedef struct {
    char Type[6];                           /* Packet type */
    GPGGA_PACKET gpgga_packet;              /* GPGGA */
    GPRMC_PACKET gprmc_packet;              /* GPRMC */
    GPGSV_PACKET gpgsv_packet;              /* GPGSV */
    int checksum;                           /* Checksum */
} GPS_PACKET;

void read_nmea_file(const char *filename, GPS_PACKET *gps_packets);

#endif //P1PROJECT_GPS_DATA_PARSER_H
