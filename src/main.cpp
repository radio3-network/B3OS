#include <Arduino.h>
#include <lvgl.h>
#include "blum.cpp"



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

    // change the builtin led for fun
    buildLedColorLoop();
    
}