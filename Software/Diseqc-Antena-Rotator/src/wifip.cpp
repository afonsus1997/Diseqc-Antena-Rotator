
#include "../include/wifip.h"

String ip_addr;

AsyncWebServer server(80);

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
    
}