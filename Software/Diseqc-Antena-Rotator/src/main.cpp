#include <Arduino.h>
#include "../include/main.h"

char *ssid = "Disecq Rotator";
char *password = "1234567890";

#pragma region temporary_oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3D
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int initOLED(){
	// Wire.begin(LCD_SDA, LCD_SCL); not needed?
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    return 1;
  return 0;
}

#pragma endregion



void initIO(){
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DWN, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_CENTER, INPUT);

  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

}

void setup() {
  Serial.begin(115200);
  initSPIFFS();
  initWifi(ssid, password, 0);
  initWebServer(); 
}

void loop() {
  // put your main code here, to run repeatedly:
}