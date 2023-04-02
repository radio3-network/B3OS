#include <Arduino.h>
#include <lvgl.h>
#include <utils/utils.h>
#include "core/blum_global.h"
#include "core/blum_widgets.h"
#include "core/blum_status_bar.h"
#include "core/blum_navigation.h"
#include "apps/blum_app_home.h"
#include "apps/blum_app_settings.h"
#include "hardware/wifi/blum_app_wifi.h"



/*

 
──▒▒▒▒▒▒───▄████▄
─▒─▄▒─▄▒──███▄█▀
─▒▒▒▒▒▒▒─▐████──█─█
─▒▒▒▒▒▒▒──█████▄
─▒─▒─▒─▒───▀████▀

███████████████████████
█▄─▄─▀█▄▄▄░█─▄▄─█─▄▄▄▄█
██─▄─▀██▄▄░█─██─█▄▄▄▄─█
▀▄▄▄▄▀▀▄▄▄▄▀▄▄▄▄▀▄▄▄▄▄▀

version 1.0.1



The B3 Operating System for Arduino.

There is an example on the setup() method to help you
getting started with the syntax. You would likely use
this library because:

    + creates standard dialogs (e.g. select wifi network)
    + easy to use button/textarea, icons
    + keyboard on screen is automatically handled 


License: Apache-2.0
Copyright (c) radio3.network
URL: https://github.com/radio3-network/BluemchenGUI/

More projects at https://github.com/radio3-network/


Reference documentation for LVGL: https://docs.lvgl.io/8/


TODO:
+ define app content area
++ respects status bar and keyboard resizing
+ clicking on textArea shows keyboard
++ clicking away from textArea hides keyboard
++ pressing keyboard button hides/toggles keyboard
+ dialog for wifi selection/save/connect
x status bar with icons (e.g. WIFI)
xx include builtin icons
+ connectivity icons on status bar
+ bluetooth, wifi, lora
+ add generic icon functionlity
++ define PNG source on SD drive
++ launch an app/script or method
++ support dark and daylight mode
+ dialog for brightness (e.g. manual/automatic)
x code for automatic brightness change
x internal RGB led effects (e.g. breathing)
+ switch between screen dark mode and daylight
+ app launcher
*/


static void loadFlashValues(){
    
    preferences.begin(NAMESPACE_GENERIC, false);
    // WIFI
    wifiEnabled = preferences.getBool(KEY_WIFI_ENABLED, false);
    //wifi_ssid = TextKeyValueStore::get(KEY_WIFI_SSID);
    //wifi_password = TextKeyValueStore::get(KEY_WIFI_PASSWORD);
    preferences.end();
    
 
}


static void start(){
    // start the hardware
    smartdisplay_init();
    // make the serial line available
    Serial.begin(115200);
    // wait for serial to be available
    while (!Serial);
    
    // print the logo
    Serial.println("");
    Serial.print(logo);
    Serial.println("");
    Serial.println("   B3 Operating System");
    Serial.println("   version " + version);
    Serial.println("");
    
    // load values in flash memory
    loadFlashValues();
    // load wifi
    wifiStart();
    // load bluetooth
    //setupBluetooth();
    // clear screen and events
    lv_obj_clean(lv_scr_act());
    mapWindows = HashMapCreate();
    // add the status bar
    buildStatusBar();
    // add the navigation
    navClean();
    // add status bar events
    addEventButtonHome();
    addEventButtonSettings();
    // initial window
    createWindowHome();
    // create the terminal
    //setupTerminal();
}

static void loopBlum(){
    lv_timer_handler();
    loopTerminal();
}

