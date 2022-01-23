#ifndef TLE_H
#define TLE_H

#include <Arduino.h>
#include <SPIFFS.h>

#define NOAA15_CATN 25338
#define NOAA18_CATN
#define NOAA19_CATN 33591
#define NOAA20_CATN 43013
#define ISS_CATN 25544

typedef struct tle_s {
   String satname;
   uint16_t CATNR;
   String tle1;
   String tle2; 
} tle_t; 

#define N_MAX_TLE 10



#endif