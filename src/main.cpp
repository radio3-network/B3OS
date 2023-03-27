#include <Arduino.h>
#include <lvgl.h>
#include "blum.cpp"

/*

██████╗ ██╗     ██╗   ██╗███████╗███╗   ███╗ ██████╗██╗  ██╗███████╗███╗   ██╗
██╔══██╗██║     ██║   ██║██╔════╝████╗ ████║██╔════╝██║  ██║██╔════╝████╗  ██║
██████╔╝██║     ██║   ██║█████╗  ██╔████╔██║██║     ███████║█████╗  ██╔██╗ ██║
██╔══██╗██║     ██║   ██║██╔══╝  ██║╚██╔╝██║██║     ██╔══██║██╔══╝  ██║╚██╗██║
██████╔╝███████╗╚██████╔╝███████╗██║ ╚═╝ ██║╚██████╗██║  ██║███████╗██║ ╚████║
╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝
                                                                              
 ██████╗ ██╗   ██╗██╗                                                         
██╔════╝ ██║   ██║██║                                                         
██║  ███╗██║   ██║██║                                                         
██║   ██║██║   ██║██║                                                         
╚██████╔╝╚██████╔╝██║                                                         
 ╚═════╝  ╚═════╝ ╚═╝                                                         
                                                                                           

The Blümchen GUI is a set of LVGL-based methods
for creating graphical dialogs without trouble.

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

*/


void setup(){
   start();
}


/**
 * Changes the builtin-LED colors in random colors
*/
static void buildLedColorLoop(){

    auto r = (byte)(millis() / 75);
    auto g = (byte)(millis() / 10);
    auto b = (byte)(millis() / 150);

    smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = b, .green = g, .red = r}}));
}

/**
 * Changes the display light intensity automatically
 * to reduce energy consumption and ease reading.
*/
static void adjustBrightnessAutomatically(){
    // 0 means full day light
    // 2300 means full darkness
    int value = smartdisplay_get_light_intensity();
    smartdisplay_tft_set_backlight(10);

}


static  int64_t lastTime = esp_timer_get_time();

static void log(String title, int value){
    Serial.print(title + ": ");
    Serial.println(value);
}

void loop(){

    loopBlum();


    //loopBluetooth();

    // change the builtin led for fun
    buildLedColorLoop();
    
    int64_t time = esp_timer_get_time();
    
    // intervals of 1 second
    if(time > (lastTime + 1000000)){
        lastTime = esp_timer_get_time();
        int value = smartdisplay_get_light_intensity();
        //log("LDR", value);
        //log("Time", lastTime);
    }

    // intervals of 5 seconds
    if(time > (lastTime + 5000000)){
        lastTime = esp_timer_get_time();
        adjustBrightnessAutomatically();
    }


}