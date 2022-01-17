#include "../include/tracker.h"

#include <Sgp4.h>
#include <Ticker.h>
#include <time.h>
//max tle size = 70

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