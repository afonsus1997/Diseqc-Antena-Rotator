#include "../include/tracker.h"
#include "../include/tle.h"
#include "../include/lcd_menu.h"
#include <math.h>


#include <Sgp4.h>
#include <Ticker.h>
#include <time.h>

#define N_ORBIT_POINTS 50

//max tle size = 70

extern uint8_t getSatIndex(uint16_t CATNR);
extern tle_t satList[N_MAX_TLE];
extern U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;

char satname[70];// = "ISS (ZARYA)";
char tle1[70];// = "1 25544U 98067A   22014.70787037  .00006419  00000-0  12245-3 0  9995"; //Line one from the TLE data
char tle2[70];// = "2 25544  51.6456  16.6326 0006618  22.6213 315.8466 15.49492476321370"; //Line two from the TLE data

uint8_t min_el = 10;

Sgp4 sat;
unsigned long unixtime = 1642196974;
int timezone = 0; //utc + 0
int year;
int mon;
int day;
int hr;
int minute;
double sec;

const char* ntpServer = "pool.ntp.org";

void Predict(int many, int minEl)
{

    passinfo overpass;                //structure to store overpass info
    sat.initpredpoint(unixtime, minEl); //finds the startpoint

    bool error;
    unsigned long start = millis();
    for (int i = 0; i < many; i++)
    {
        error = sat.nextpass(&overpass, 20); //search for the next overpass, if there are more than 20 maximums below the horizon it returns false
        delay(0);

        if (error == 1)
        { //no error, prints overpass information

            invjday(overpass.jdstart, timezone, true, year, mon, day, hr, minute, sec);
            Serial.println("Overpass " + String(day) + ' ' + String(mon) + ' ' + String(year));
            Serial.println("  Start: el=" + String(overpass.minelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));

            invjday(overpass.jdmax, timezone, true, year, mon, day, hr, minute, sec);
            Serial.println("  Max: elev=" + String(overpass.maxelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));

            invjday(overpass.jdstop, timezone, true, year, mon, day, hr, minute, sec);
            Serial.println("  Stop: el=" + String(overpass.minelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));

            switch (overpass.transit)
            {
            case none:
                break;
            case enter:
                invjday(overpass.jdtransit, timezone, true, year, mon, day, hr, minute, sec);
                Serial.println("  Enter earth shadow: " + String(hr) + ':' + String(minute) + ':' + String(sec));
                break;
            case leave:
                invjday(overpass.jdtransit, timezone, true, year, mon, day, hr, minute, sec);
                Serial.println("  Leave earth shadow: " + String(hr) + ':' + String(minute) + ':' + String(sec));
                break;
            }
            switch (overpass.sight)
            {
            case lighted:
                Serial.println("  Visible");
                break;
            case eclipsed:
                Serial.println("  Not visible");
                break;
            case daylight:
                Serial.println("  Daylight");
                break;
            }
        }
        else
        {
            Serial.println("Prediction error");
        }
        delay(0);
    }
    unsigned long einde = millis();
    Serial.println("Time: " + String(einde - start) + " milliseconds");
}

void init_tracker()
{
    configTime(0, 0, ntpServer);
    time_t now;
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    unixtime = time(&now);

    sat.site(38.7243, -9.13, 0); //set site latitude[°], longitude[°] and altitude[m]
    // char satname[] = "ISS (ZARYA)";
    // char tle_line1[] = "1 25544U 98067A   22014.70787037  .00006419  00000-0  12245-3 0  9995"; //Line one from the TLE data
    // char tle_line2[] = "2 25544  51.6456  16.6326 0006618  22.6213 315.8466 15.49492476321370"; //Line two from the TLE data



    sat.init(satname, tle1, tle2); //initialize satellite parameters

    //Display TLE epoch time
    double jdC = sat.satrec.jdsatepoch;
    invjday(jdC, timezone, true, year, mon, day, hr, minute, sec);
    Serial.println("Epoch: " + String(day) + '/' + String(mon) + '/' + String(year) + ' ' + String(hr) + ':' + String(minute) + ':' + String(sec));
    Serial.println();
    Predict(5, 10);
}

void predict_eng_init_menu(uint16_t CATN){
    
    uint8_t satIndex = getSatIndex(CATN);
    String satname_s = satList[satIndex].satname;
    String tle1_s = satList[satIndex].tle1;
    String tle2_s = satList[satIndex].tle2;

    configTime(0, 0, ntpServer);
    time_t now;
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    unixtime = time(&now);

    sat.site(38.7243, -9.13, 0);

    

    satname_s.toCharArray(satname, satname_s.length());
    tle1_s.toCharArray(tle1, tle1_s.length());
    tle2_s.toCharArray(tle2, tle2_s.length());

    sat.init(satname, tle1, tle2);

}

void predict_eng_show_menu(int many, int minEl){
    passinfo overpass;                //structure to store overpass info
    sat.initpredpoint(unixtime, minEl);

    bool error;

    

    for (int i = 0; i < many; i++)
    {
        error = sat.nextpass(&overpass, 20); //search for the next overpass, if there are more than 20 maximums below the horizon it returns false
    //     delay(0);

        if (error == 1)
        { //no error, prints overpass information

            invjday(overpass.jdstart, timezone, true, year, mon, day, hr, minute, sec);
            u8g2.clearDisplay();
            // u8g2.nextPage();

            String builder;
            u8g2.setDrawColor(2);
            u8g2.setFontMode(1);
            Serial.println(builder);
            u8g2.setCursor(0, fontY); 
            u8g2.print(String(day) + '/' + String(mon) + '/' + String(year));

            u8g2.setCursor(0,fontY*2);
            u8g2.print("S: el=" + String(overpass.minelevation, 1) + " " + String(hr) + ':' + String(minute));

            // Serial.println("Overpass " + String(day) + ' ' + String(mon) + ' ' + String(year));
            // Serial.println("  Start: el=" + String(overpass.minelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));

            invjday(overpass.jdmax, timezone, true, year, mon, day, hr, minute, sec);
            // Serial.println("  Max: elev=" + String(overpass.maxelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));
            u8g2.setCursor(0,fontY*3);
            u8g2.print("   el=" + String(overpass.maxelevation, 1) + " " + String(hr) + ':' + String(minute));

            invjday(overpass.jdstop, timezone, true, year, mon, day, hr, minute, sec);
            // Serial.println("  Stop: el=" + String(overpass.minelevation) + "° " + String(hr) + ':' + String(minute) + ':' + String(sec));
            u8g2.setCursor(0,fontY*4);
            u8g2.print("E: el=" + String(overpass.minelevation, 1) + " " + String(hr) + ':' + String(minute));
            while(u8g2.nextPage());


    //         switch (overpass.transit)
    //         {
    //         case none:
    //             break;
    //         case enter:
    //             invjday(overpass.jdtransit, timezone, true, year, mon, day, hr, minute, sec);
    //             Serial.println("  Enter earth shadow: " + String(hr) + ':' + String(minute) + ':' + String(sec));
    //             break;
    //         case leave:
    //             invjday(overpass.jdtransit, timezone, true, year, mon, day, hr, minute, sec);
    //             Serial.println("  Leave earth shadow: " + String(hr) + ':' + String(minute) + ':' + String(sec));
    //             break;
    //         }
    //         switch (overpass.sight)
    //         {
    //         case lighted:
    //             Serial.println("  Visible");
    //             break;
    //         case eclipsed:
    //             Serial.println("  Not visible");
    //             break;
    //         case daylight:
    //             Serial.println("  Daylight");
    //             break;
    //         }
        }
    //     else
        {
    //         Serial.println("Prediction error");
        }
        delay(1000);

        draw_orbit(getUnixFromJulian(overpass.jdstart), getUnixFromJulian(overpass.jdstop), 64, 32, 30);
        delay(1000);



    }

}

void draw_orbit(unsigned long pass_start, unsigned long pass_end, uint8_t cx, uint8_t cy, uint8_t size){
    
    uint16_t step = (pass_end - pass_start)/N_ORBIT_POINTS;

    u8g2.setDrawColor(2);
    u8g2.setFontMode(1);
    u8g2.clearDisplay();
    u8g2.drawCircle(cx, cy, size);
    u8g2.drawCircle(cx, cy, size/2);
    u8g2.drawLine(cx, cy-size, cx, size+cy);
    u8g2.drawLine(cx-size, cy, size+cx, cy);

    float XTheta, YTheta, XPhi, YPhi, X, Y;

    // Serial.println("Pass orbit: ");
    uint8_t i;
    uint8_t orbit_points_x[N_ORBIT_POINTS], orbit_points_y[N_ORBIT_POINTS];
    for(i = 0; i<N_ORBIT_POINTS; i++){
        sat.findsat(pass_start + i*step);
        // Serial.println("El: " + String( sat.satEl) + " Az: " + String( sat.satAz));
        X = cos((sat.satEl*M_PI)/180) * sin((sat.satAz*M_PI)/180) * size;
        Y = - cos((sat.satAz*M_PI)/180) * cos((sat.satEl*M_PI)/180) * size;
        // Serial.println("X: " + String(X) + " Y: " + String(Y));
        orbit_points_x[i] = (uint8_t)X + cx;
        orbit_points_y[i] = (uint8_t)Y + cy;
    }

    for(i=0; i<N_ORBIT_POINTS; i++){
        u8g2.drawPixel(orbit_points_x[i], orbit_points_y[i]);
    }
     while(u8g2.nextPage());
}

void track_eng_menu(int minEl){

    configTime(0, 0, ntpServer);
    time_t now;
    struct tm timeinfo;
    getLocalTime(&timeinfo);


    unixtime = 1643043600;//time(&now);

    unsigned long pass_start, pass_end, step;
    passinfo overpass;                //structure to store overpass info
    sat.initpredpoint(unixtime, minEl);
    sat.nextpass(&overpass, 20);

    pass_start = getUnixFromJulian(overpass.jdstart);
    pass_end = getUnixFromJulian(overpass.jdstop);

    draw_orbit(pass_start, pass_end, 64, 32, 32);

    // Serial.println("Start epoch: " + String(getUnixFromJulian(overpass.jdstart)));
    // Serial.println("Stop epoch: " + String(getUnixFromJulian(overpass.jdstop)));

    sat.findsat(unixtime);

    invjday(sat.satJd , timezone,true, year, mon, day, hr, minute, sec);
    // Serial.println(String(day) + '/' + String(mon) + '/' + String(year) + ' ' + String(hr) + ':' + String(minute) + ':' + String(sec));
    // Serial.println("azimuth = " + String( sat.satAz) + " elevation = " + String(sat.satEl) + " distance = " + String(sat.satDist));
    // Serial.println("latitude = " + String( sat.satLat) + " longitude = " + String( sat.satLon) + " altitude = " + String( sat.satAlt));

    switch(sat.satVis){
        case -2:
            Serial.println("Visible : Under horizon");
            break;
        case -1:
            Serial.println("Visible : Daylight");
            break;
        default:
            Serial.println("Visible : " + String(sat.satVis));   //0:eclipsed - 1000:visible
            break;
        }
}