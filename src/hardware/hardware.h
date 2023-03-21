#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include "bluetooth/bluetooth.cpp"


#include <mutex>

// Mutex to access lvgl if multi-threaded
extern std::recursive_mutex lvgl_mutex;
// Initialize the display and touch
extern void smartdisplay_init();
// Set the color of the led
extern void smartdisplay_set_led_color(lv_color32_t rgb);
// Get the value of the CDS sensor
extern int smartdisplay_get_light_intensity();
// Beep with the specified frequency and duration
extern void smartdisplay_beep(unsigned int frequency, unsigned long duration);
// Set the brightness of the backlight display
extern void smartdisplay_tft_set_backlight(uint16_t duty); // 0-1023 (12 bits)
// Put the display to sleep
extern void smartdisplay_tft_sleep();
// Wake the display 
extern void smartdisplay_tft_wake();

#include "choose.h"

#endif