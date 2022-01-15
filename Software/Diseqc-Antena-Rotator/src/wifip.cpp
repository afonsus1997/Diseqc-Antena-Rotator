
#include "../include/wifip.h"

String ip_addr;

AsyncWebServer server(80);
HTTPClient http;
//ISS 25544

extern char satname[70];// = "ISS (ZARYA)";
extern char tle1[70];// = "1 25544U 98067A   22014.70787037  .00006419  00000-0  12245-3 0  9995"; //Line one from the TLE data
extern char tle2[70];

int getTLE(uint16_t CATNR)
{
    String payload;
    http.begin("https://celestrak.com/satcat/tle.php?CATNR=" + String(CATNR)); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
    if (httpCode > 0)
    { //Check for the returning code
        payload = http.getString();
        Serial.println(httpCode);
        // Serial.println(payload.substring(0));
    }
    else
    {
        Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources

    Serial.println(payload);

    size_t head, tail;
    String satname_s, tle1_s, tle2_s;
    head = payload.indexOf('\n');
    satname_s = payload.substring(0, head-1); //from -> to
    payload = payload.substring(head+1);
    Serial.println(satname_s);

    head = payload.indexOf('\n');
    tle1_s = payload.substring(0, head-1);
    payload = payload.substring(head+1);
    Serial.println(tle1_s);

    head = payload.indexOf('\n');
    tle2_s = payload.substring(0, head-1);
    Serial.println(tle2_s);
    
    satname_s.toCharArray(satname, satname_s.length());
    tle1_s.toCharArray(tle1, tle1_s.length());
    tle2_s.toCharArray(tle2, tle2_s.length());


    //this took way longer than it should and its ugly af

}

int initWebHandles()
{
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server.on("/SendString", handleStringTxt);
    // server.on("/SpacingString", handleStringSpacing);
    // server.on("/SpeedString", handleStringSpeed);
    // server.on("/BrightnessString", handleStringBrightness);

    return 1;
}

int initWebServer()
{
    Serial.println("Starting web server...");
    initWebHandles();
    server.begin();
    return 1;
}

int initWifi(char *ssid, char *password, uint16_t mode)
{
    Serial.print("Initializing Wifi...");
    if (mode)
    {
        WiFi.softAP("Pixels Camp Badger", "1234567890");
        IPAddress myIP = WiFi.softAPIP();
        ip_addr = myIP.toString();
        Serial.println("Done!");
        //initDNS("pixelsbadger");
        Serial.print("Initializing Webserver...");
        // initWebServer();
        Serial.println("Done!");
        Serial.println("==============");
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Password: ");
        Serial.println(password);
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        Serial.println("==============\n");
    }
    else
    {
        WiFi.mode(WIFI_STA); //Optional
        WiFi.begin(SSID, PASSWORD);
        Serial.println("\nConnecting");

        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(100);
        }

        Serial.println("\nConnected to the WiFi network");
        Serial.print("Local ESP32 IP: ");
        Serial.println(WiFi.localIP().toString());
        ip_addr = WiFi.localIP().toString();
    }
    getTLE(25544);
}