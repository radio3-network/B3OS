
#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"
//#include "blum_app_home.h"
//#include "blum_app_settings.h"


static void statusBarBackButton(){
  lv_label_set_text(labelSettingsButton, LV_SYMBOL_LEFT);
}

static void statusBarSettingsButton(){
  lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
}

/**
 * Update the status bar text
*/
static void statusBarTextUpdate(const char* text){
    lv_label_set_text(statusTextLabel, text );
}


