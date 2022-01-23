#include "../include/lcd_menu.h"

extern uint8_t min_el;
extern void predict_eng_init_menu(uint16_t CATN);
extern void predict_eng_show_menu(int many, int minEl);
extern uint8_t getSatIndex(uint16_t CATNR);

#define fontName u8g2_font_7x13_mf

#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64
#define USE_HWI2C
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);//, SCL, SDA);
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, 22, 21);
U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);
// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, LCD_SCL, LCD_SDA);


#define USE_SSD1306
using namespace Menu;
#define LEDPIN LED_BUILTIN
const colorDef<uint8_t> colors[6] MEMMODE = {
    {{0, 0}, {0, 1, 1}}, //bgColor
    {{1, 1}, {1, 0, 0}}, //fgColor
    {{1, 1}, {1, 0, 0}}, //valColor
    {{1, 1}, {1, 0, 0}}, //unitColor
    {{0, 1}, {0, 0, 1}}, //cursorColor
    {{1, 1}, {1, 0, 0}}, //titleColor
};

char *constMEM hexDigit MEMMODE = "0123456789ABCDEF";
char *constMEM hexNr[] MEMMODE = {"0", "x", hexDigit, hexDigit};
char buf1[] = "0x11";

result optest(){Serial.println("done!\n");return proceed;}
result doAlert(eventMask e, prompt &item);


int wifi_mode_set_menu=0;
CHOOSE(wifi_mode_set_menu,chooseWifiMenu,"Wifi Mode",doNothing,noEvent,noStyle
  ,VALUE("Wifi AP",0,doNothing,noEvent)
  ,VALUE("WIFI B",1,doNothing,noEvent)
);


MENU(wifi_submenu,"Wifi Settings",doNothing,noEvent,noStyle
  ,SUBMENU(chooseWifiMenu)
  ,OP("Check IP",doAlert,enterEvent)
  ,EXIT("<Back")
);

int8_t el_offset = 0;
int8_t az_offset = 0;


MENU(rotator_ctrl_submenu,"Rotator control",doNothing,noEvent,noStyle
  ,OP("Park antenna", doNothing, noEvent)
  ,FIELD(el_offset,"El offset"," deg", -90,90,1,5, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,FIELD(az_offset,"Az offset"," deg", 0,90,1,5, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,EXIT("<Back")
);


uint16_t predict_sat_catn=NOAA15_CATN;
CHOOSE(predict_sat_catn,choose_track_sat_menu,"Choose sat",doNothing,noEvent,noStyle
  ,VALUE("NOAA 15",NOAA15_CATN,doNothing,noEvent)
  ,VALUE("NOAA 18",NOAA18_CATN,doNothing,noEvent)
  ,VALUE("NOAA 19",NOAA19_CATN,doNothing,noEvent)
  ,VALUE("ISS",ISS_CATN,doNothing,noEvent)
);

uint8_t n_predictions = 1;


result menu_predict();

MENU(predictor_submenu,"Prediction engine",doNothing,noEvent,noStyle
  ,SUBMENU(choose_track_sat_menu)
  ,FIELD(min_el,"Min el"," deg", 0,90,1,0, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,FIELD(n_predictions,"N predictions","", 1,10,1,0, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,OP("Predict!", menu_predict, enterEvent)
  ,EXIT("<Back")
);



MENU(mainMenu, "Main menu"\
    ,doNothing
    ,noEvent
    ,wrapStyle
    ,OP("Op1", optest, enterEvent)
    ,OP("Op2", doNothing, noEvent)
    ,SUBMENU(rotator_ctrl_submenu)
    ,SUBMENU(predictor_submenu)
    ,SUBMENU(wifi_submenu)
    ,EXIT("<Exit"));

#define MAX_DEPTH 4

serialIn serial(Serial);
MENU_INPUTS(in, &serial);
// MENU_INPUTS(in,&encStream,&encButton);//,&serial);

MENU_OUTPUTS(out, MAX_DEPTH, U8G2_OUT(u8g2, colors, fontX, fontY, offsetX, offsetY, {0, 0, U8_Width / fontX, U8_Height / fontY}), SERIAL_OUT(Serial));

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

void show_prediction(int i){
  u8g2.clearDisplay();
  
  u8g2.setCursor(10,10);
  u8g2.print("Prediction");
  u8g2.nextPage();
  
    
}


result menu_predict(){
  u8g2.setDrawColor(0);
  u8g2.setFontMode(1);

  predict_eng_init_menu(predict_sat_catn);
  predict_eng_show_menu(n_predictions, min_el);
    
  return proceed;
}


result alert(menuOut &o, idleEvent e)
{
    if (e == idling)
    {
        o.setCursor(0, 0);
        o.print("Current IP address is:");
        o.setCursor(0, 1);
        o.print("192.168.1.111");
        o.setCursor(0, 2);
        o.print("press [select]");
        o.setCursor(0, 3);
        o.print("to continue...");
    }
    return proceed;
}

result doAlert(eventMask e, prompt &item)
{
    nav.idleOn(alert);
    return proceed;
}

//when menu is suspended
result idle(menuOut &o, idleEvent e)
{
    o.clear();
    switch (e)
    {
    case idleStart:
        o.println("suspending menu!");
        break;
    case idling:
        o.println("suspended...");
        break;
    case idleEnd:
        o.println("resuming menu.");
        break;
    }
    return proceed;
}

const navCodesDef myCodes = {
    {noCmd, (char)0xff},
    {escCmd, 'q'},
    {enterCmd, 's'},
    {upCmd, 'x'},
    {downCmd, 'w'},
    {leftCmd, 'a'},
    {rightCmd, 'd'},
    {idxCmd, '?'},
    {scrlUpCmd, 0x35},
    {scrlUpCmd, 0x36}};

config myOptions('*', '-', myCodes, false);

void initOLED()
{
    options = &myOptions;
    // Wire.begin();
    u8g2.begin();
    u8g2.setFont(fontName);
    u8g2.setDrawColor(2);
    u8g2.setFontMode(1);
    // u8g2.drawBox(5,5, U8_Width, U8_Height);
    // while(u8g2.nextPage());
    u8g2.drawStr(10,30, "asdasdasdasdasdasd");
    // u8g2.print("asdasdasdasdasdasd");
    

    
}

void menuLoop(){
    nav.doInput();
  // digitalWrite(LEDPIN, ledCtrl);
    if (nav.changed(0)) {//only draw if menu changed for gfx device
    //change checking leaves more time for other tasks
        u8g2.firstPage();
        do nav.doOutput(); while(u8g2.nextPage());

  }
  if (nav.sleepTask) {
    u8g2.setCursor(10,10);
    u8g2.print("Status screen");
    u8g2.nextPage();
  }
}