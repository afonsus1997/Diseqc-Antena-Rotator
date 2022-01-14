#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "boardpins.h"

#include <menu.h>
#include <menuIO/u8g2Out.h>
// #include <menuIO/encoderIn.h>
// #include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>

#include <Wire.h>



void initOLED();
void menuLoop();

#endif