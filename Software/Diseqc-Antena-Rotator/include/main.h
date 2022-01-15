#ifndef MAIN_H
#define MAIN_H
/*Arduino INCLUDES*/
#include <Arduino.h>



/*WIFI INCLUDES*/
extern int initWifi(char *ssid, char *password, uint16_t mode);
extern int initWebServer(); 

/*SPIFFS INCLUDES*/
int initSPIFFS();

/*BOARD AND IO INCLUDES*/
#include "boardpins.h"

extern void initOLED();
extern void menuLoop();
extern void init_tracker(); 

#endif