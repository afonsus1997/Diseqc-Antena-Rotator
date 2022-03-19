#include "../include/tle.h"


extern int getTLE(String CATNR);

#define N_DEFAULT_SATS 5
uint16_t default_sats[N_DEFAULT_SATS] = {NOAA15_CATN, NOAA18_CATN, NOAA20_CATN, ISS_CATN};

tle_t satList[N_MAX_TLE];
uint8_t n_LoadedTLEs;


void write_tles_to_file(){
    File file = SPIFFS.open("/tle.txt", FILE_WRITE);
    uint8_t i;
    Serial.println("writing new data to spiffs");
    for(i = 0; i<n_LoadedTLEs; i++){
        file.println(satList[i].satname);
        file.println(String(satList[i].CATNR));
        file.println(satList[i].tle1);
        file.println(satList[i].tle2);
    }
    file.print('\n');
    file.close();

    file = SPIFFS.open("/tle.txt", FILE_READ);
    Serial.println(file.readString());
    file.close();
    
}

void add_tle_to_list(String satname, uint16_t CATNR, String tle1_s, String tle2_s){
    uint8_t found_flag = 0;
    uint8_t i;
    Serial.println(String(CATNR) + "\n-------\n");
    for(i = 0; i<N_MAX_TLE; i++){
        Serial.println(satList[i].CATNR);
        if(satList[i].CATNR == CATNR){
            Serial.println("found it");
            found_flag = 1;
            break;
        }
    }

    if(found_flag){
        satList[i].CATNR = CATNR;
        satList[i].satname = satname;
        satList[i].tle1 = tle1_s;
        satList[i].tle2 = tle2_s;
        Serial.println("List updated");
        write_tles_to_file();
        Serial.println("TLE file written!");
    }else{
        Serial.println("Sat not found, adding it to the list.");
        satList[n_LoadedTLEs].CATNR = CATNR;
        Serial.println(String(CATNR));
        satList[n_LoadedTLEs].satname = satname;
        Serial.println(satname);
        satList[n_LoadedTLEs].tle1 = tle1_s;
        Serial.println(tle1_s);
        satList[n_LoadedTLEs].tle2 = tle2_s;
        Serial.println(tle2_s);

        n_LoadedTLEs++;
        Serial.println("List updated");
        write_tles_to_file();
    }
        
}

uint8_t get_tles_from_file(){
    Serial.println("Reading TLE from SPIFFS");
    File file = SPIFFS.open("/tle.txt", FILE_READ);
    Serial.println(file.readString());
    file.close();
    file = SPIFFS.open("/tle.txt", FILE_READ);
    String line;
    uint8_t i = 0;
    char nc;
    for(i = 0; i < N_MAX_TLE; i++){
        nc = file.peek();
        if(nc == ' ' || nc == '\n'){ //this mean end of file
            Serial.println("Done reading spiffs. Loaded " + String(i) + " sats!");
            break;
        }
        else{
            Serial.println("peeked char " + String(nc));
            line = file.readStringUntil('\n');
            satList[i].satname = line;
            Serial.println(satList[i].satname);
        }

        line = file.readStringUntil('\n');
        satList[i].CATNR = line.toInt();
        // Serial.println(satList[i].CATNR);

        line = file.readStringUntil('\n');
        satList[i].tle1 = line;
        // Serial.println(satList[i].tle1);

        line = file.readStringUntil('\n');
        satList[i].tle2 = line;
        // Serial.println(satList[i].tle2);

    }

    Serial.println(i);
    file.close();
    return i;
}

uint8_t getSatIndex(uint16_t CATNR){
    uint8_t i;
    for(i = 0; i<N_MAX_TLE; i++){
        Serial.println(satList[i].CATNR);
        if(satList[i].CATNR == CATNR){
            Serial.println("found it");
            return i;
        }
    }
    return 0;
}



void initTLEs(){
    n_LoadedTLEs = get_tles_from_file();
    
}

