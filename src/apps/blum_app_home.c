#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"
#include "blum_widgets.h"
#include "blum_status_bar.h"



static void createWindowHome(){
    // create a usable window to place components
    homeWindow = createWindow("Home");
}


/**
 * Home click
*/
static void btn_event_home(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED){
       createWindowHome();
    }
}



static void addEventButtonHome(){
    lv_obj_add_event_cb(homeBtn, btn_event_home, LV_EVENT_CLICKED, NULL);
}



