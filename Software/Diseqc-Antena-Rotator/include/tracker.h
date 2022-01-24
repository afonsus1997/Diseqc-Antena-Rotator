#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>

#define MAX_TLE_SIZE 70

void init_tracker();

void draw_orbit(unsigned long pass_start, unsigned long pass_end, uint8_t cx, uint8_t cy, uint8_t size);

#endif