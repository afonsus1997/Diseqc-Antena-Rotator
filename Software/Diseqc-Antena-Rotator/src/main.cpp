#include <Arduino.h>
#include "../include/main.h"

char *ssid = "Disecq Rotator";
char *password = "1234567890";

#pragma region temporary_oled

// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <Wire.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define SCREEN_ADDRESS 0x78
// #define OLED_RESET -1

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// int initOLED(){
// 	// Wire.begin(LCD_SDA, LCD_SCL); not needed?
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//     Serial.println(F("SSD1306 allocation failed"));
//     return 1;
//   }

//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=2) {
//     display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   return 0;
// }

#pragma endregion




#pragma region temporary_menu



constexpr int OLED_SDA=21;
constexpr int OLED_SCL=22;
#define I2C_ADDRESS 0x78







// //customizing a prompt look!
// //by extending the prompt class
// class altPrompt:public prompt {
// public:
//   altPrompt(constMEM promptShadow& p):prompt(p) {}
//   Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
//     return out.printRaw(F("special prompt!"),len);;
//   }
// };









// encoderIn<encA,encB> encoder;//simple quad encoder driver
// encoderInStream<encA,encB> encStream(encoder,4);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
// keyMap encBtn_map[]={{-encBtn,defaultNavCodes[enterCmd].ch}};//negative pin numbers use internal pull-up, this is on when low
// keyIn<1> encButton(encBtn_map);//1 is the number of keys

// menuIn* inputsList[]={&encBuitton,&Serial};
// chainStream<2> in(inputsList);//1 is the number of inputs



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
  // initWebServer(); 
  // initOLED();
  init_tracker();

}

void loop() {
  // put your main code here, to run repeatedly:
  // menuLoop();
  delay(100);//simulate other tasks delay
}