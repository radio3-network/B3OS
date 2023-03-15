#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"

#ifndef BLUM_STATUS_BAR_H
#include "blum_status_bar.h"
#endif



static void event_settings_not_implemented(lv_event_t *e){
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, 
    "Info", "Not yet implemented", NULL, true);
    lv_obj_center(mbox1);
}