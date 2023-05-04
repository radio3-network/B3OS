/*

This project assumes that wifi modules are built inside
the board such as the case for ESP32 models.

Therefore provides no support for pin connections.

*/

#include <Preferences.h>
#include "core/blum_global.h"
#include "core/blum_status_bar.h"

void wifiStop() {
    // remove the wifi icon
    iconWifiRemove();
    // disconnect the wifi
    WiFi.disconnect();

    // mark this setting in permanent memory
    preferences.begin(NAMESPACE_GENERIC, false);
    wifiEnabled = preferences.putBool(KEY_WIFI_ENABLED, false);
    preferences.end();

    // mark this setting on runtime memory
    wifiEnabled = false;

    Serial.println("WiFi disconnected");
}

boolean wifiStart() {
    Serial.println("Starting WiFi");
    // remove the wifi icon at the beginning
    iconWifiRemove();

    // read from flash memory
    preferences.begin(NAMESPACE_GENERIC, false);

    // there is no wifi to start
    if (wifiEnabled == false) {
        Serial.println("WiFi is disabled");
        return false;
    }

     if(preferences.isKey(KEY_WIFI_ENABLED) == false){
        Serial.println("There is no WiFi previously configured");
        return false;
     }

    // preferences.putBool(KEY_WIFI_ENABLED, enabledWifi);
    //  if wifi enabled, set it to true
    wifiEnabled = preferences.getBool(KEY_WIFI_ENABLED, false);

    // read the SSID and password
    Serial.println("Reading wifi SSID and password from flash memory");
    wifi_ssid = preferences.getString(KEY_WIFI_SSID, "");
    wifi_password = preferences.getString(KEY_WIFI_PASSWORD, "");

    // no more need to read stuff from flash
    preferences.end();

   

    if (wifi_ssid.isEmpty()) {
        Serial.println("WiFi needs to be configured, please connect to a reachable network");
        return false;
    }

    if (wifi_password.isEmpty()) {
        Serial.println("WiFi password needs to be configured, please connect to a reachable network");
        return false;
    }

    Serial.print("Connecting to  WiFi: ");
    Serial.println(wifi_ssid);
    Serial.print("Password: ");
    Serial.println(wifi_password);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(WIFI_PS_NONE);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

    int maxTries = 5;
    int i = 0;

    while (WiFi.status() != WL_CONNECTED) {
        i = i + 1;
        delay(1000);
        Serial.print(".");
        if (i > maxTries) {
            Serial.println("");
            Serial.println("WiFi connection failed");
            return false;
        }
    }
    Serial.println("WiFi connected");
    Serial.print("Device IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" with terminal at port: ");
    Serial.println(SERVER_PORT);

    boolean isConnected = isWiFiConnected();

    // create the icon on the status bar, only when connected
    if (isConnected) {
        iconWifiCreate();
        wifiEnabled = true;
    }else{
        wifiEnabled = false;
    }
    // are we really connected?
    return isConnected;
}

boolean isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}