#include <Arduino.h>
#include <EEPROM.h>
#include <lvgl.h>
#include <esp32_smartdisplay.h>
#include "blum.c"

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

/*

// hardware-specific definitions

static int screen_width = 320;
static int screen_height = 240;


*/


/**
 * Changes the builtin-LED colors in random colors
*/
static void buildLedColorLoop(){

    auto r = (byte)(millis() / 75);
    auto g = (byte)(millis() / 10);
    auto b = (byte)(millis() / 150);

    smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = b, .green = g, .red = r}}));
}

void btn_event_button1(lv_event_t *e)
{
    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}



void tileView(void)
{
    // create the tile view
    lv_obj_t *tv = lv_tileview_create(lv_scr_act());
    // use all screen except the top because of the status bar
    lv_obj_set_size(tv, LV_HOR_RES, LV_VER_RES- statusBarWeight);
    lv_obj_align(tv, LV_ALIGN_BOTTOM_MID, 0, 0);

    /*Tile1: just a label*/
    lv_obj_t * tile1 = lv_tileview_add_tile(tv, 0, 0, LV_DIR_BOTTOM);
    lv_obj_t * label = lv_label_create(tile1);
    lv_label_set_text(label, "Scroll down");
    lv_obj_center(label);


    /*Tile2: a button*/
    lv_obj_t * tile2 = lv_tileview_add_tile(tv, 0, 1, LV_DIR_TOP | LV_DIR_RIGHT);

    lv_obj_t * btn = lv_btn_create(tile2);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Scroll up or right");

    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(btn);

    /*Tile3: a list*/
    lv_obj_t * tile3 =  lv_tileview_add_tile(tv, 0, 2, LV_DIR_LEFT);
    lv_obj_t * list = lv_list_create(tile3);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));

    lv_list_add_btn(list, NULL, "One");
    lv_list_add_btn(list, NULL, "Two");
    lv_list_add_btn(list, NULL, "Three");
    lv_list_add_btn(list, NULL, "Four");
    lv_list_add_btn(list, NULL, "Five");
    lv_list_add_btn(list, NULL, "Six");
    lv_list_add_btn(list, NULL, "Seven");
    lv_list_add_btn(list, NULL, "Eight");
    lv_list_add_btn(list, NULL, "Nine");
    lv_list_add_btn(list, NULL, "Ten");
}








void setup()
{
    Serial.begin(115200);

    // initialize the hardware
    smartdisplay_init();


    // Clear screen
    lv_obj_clean(lv_scr_act());
    buildStatusBar();
    //tileView();
    createWindowHome();
    //createWindowSettings();

    // Create a button
    //createButton(10, 10, 120, 50, "Teste", btn_event_button1);
    //createButton(10, 70, 120, 50, "Keyboard", btn_event_togglekeyboard);
    /*
    const char* items = 
        "TESTE one\n"
        "TESTE two\n"
        "TESTE three\n"
        "TESTE four\n"
        "TEST five\n"
        "TEST six";
    */
    //createRoller(140, 10, 160, 100, items, NULL);
    //auto textArea = createTextArea(140, 10, 160, 100, "Write here..");

    //keyboardShow(textArea, NULL);

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

void loop()
{
    lv_timer_handler();

    // change the builtin led for fun
    buildLedColorLoop();
    
    int64_t time = esp_timer_get_time();
    
    // intervals of 1 second
    if(time > (lastTime + 1000000)){
        lastTime = esp_timer_get_time();
        int value = smartdisplay_get_light_intensity();
        log("LDR", value);
        log("Time", lastTime);
    }

    // intervals of 5 seconds
    if(time > (lastTime + 5000000)){
        lastTime = esp_timer_get_time();
        adjustBrightnessAutomatically();
    }


}