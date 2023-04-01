/*

This project assumes that wifi modules are built inside
the board such as the case for ESP32 models.

Therefore provides no support for pin connections.

*/

#include <Preferences.h>
#include <ArduinoJson.h>
#include "core/blum_global.h"

void wifiStart()
{

    // read from flash memory
    preferences.begin(NAMESPACE_GENERIC, false);
    // preferences.putBool(KEY_WIFI_ENABLED, enabledWifi);
    //  if wifi enabled, set it to true
    wifiEnabled = preferences.getBool(KEY_WIFI_ENABLED, false);

    if(preferences.isKey(KEY_WIFI_SSID) 
        && preferences.isKey(KEY_WIFI_PASSWORD)){
        wifi_ssid = preferences.getString(KEY_WIFI_SSID);
        wifi_password = preferences.getString(KEY_WIFI_PASSWORD);
    }

    // no more need to read stuff from flash
    preferences.end();

    // there is no wifi to start
    if (wifiEnabled == false){
        return;
    }

    if(wifi_ssid.isEmpty()){
        Serial.println("WiFi needs to be configured, please connect to a reachable network");
        return;
    }

    Serial.print("Connecting to  WiFi: ");
    Serial.print(wifi_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(WIFI_PS_NONE);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

    int maxTries = 100;
    int i = 0;

    while (WiFi.status() != WL_CONNECTED){
        i++;
        delay(100);
        Serial.print(".");
        if(i > maxTries){
            Serial.println("");
            Serial.println("WiFi connection failed");
            return;
        }
    }
    Serial.println("WiFi connected");
    Serial.print( "Device IP: " );
    Serial.print( WiFi.localIP() );
    Serial.print( " with terminal at port: " );
    Serial.println( SERVER_PORT );

}

boolean isWiFiConnected(){
    return WiFi.status() == WL_CONNECTED;
}