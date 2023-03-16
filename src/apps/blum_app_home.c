#include <Arduino.h>
#include <lvgl.h>
#include "core/blum_global.h"
#include "core/blum_widgets.h"
#include "core/blum_status_bar.h"
#include "core/blum_navigation.h"



static void createWindowHome(){
    // delete the memory map
    HashMapClear(mapWindows);
    // restore the icon and index
    navClean();
    // create a usable window to place components
    homeWindow = createWindow("Home");
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



