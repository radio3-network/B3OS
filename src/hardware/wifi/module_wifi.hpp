/*

This project assumes that wifi modules are built inside
the board such as the case for ESP32 models.

Therefore provides no support for pin connections.

*/

#ifndef MODULE_WIFI_HPP
#define MODULE_WIFI_HPP


extern boolean wifiStart();
extern void wifiStop();
extern boolean isWiFiConnected();

#endif