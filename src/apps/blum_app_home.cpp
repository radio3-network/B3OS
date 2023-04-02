#include <Arduino.h>
#include <lvgl.h>
#include "core/blum_global.h"
#include "core/blum_widgets.h"
#include "core/blum_status_bar.h"
#include "core/blum_navigation.h"


static void btn_event_cb(lv_event_t *e)
{

    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;
        SerialWriteLn("Pressed");
        //logWrite("Pressed");
        
        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}



static void createWindowHome(){
    // delete the memory map
    HashMapClear(mapWindows);
    // restore the icon and index
    navClean();
    // create a usable window to place components
    homeWindow = createWindow("Home");

    // add some testing info
    auto btnDebug = lv_btn_create(homeWindow);
    lv_obj_set_pos(btnDebug, 10, 10);
    lv_obj_set_size(btnDebug, 120, 50);
    lv_obj_align(btnDebug, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btnDebug, btn_event_cb, LV_EVENT_ALL, NULL);
    // Set label to Button
    auto label = lv_label_create(btnDebug);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
}


/**
 * Home click
*/
static void btn_event_home(lv_event_t *e){
    // reset everything
    navClean();
    // create the new window again
    createWindowHome();
}



static void addEventButtonHome(){
    lv_obj_add_event_cb(homeBtn, btn_event_home, LV_EVENT_CLICKED, NULL);
}



