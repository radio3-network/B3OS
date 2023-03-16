#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"
#include "blum_status_bar.h"
#include "apps/blum_app_home.h"
#include "apps/blum_app_wifi.h"

static void event_settings_not_implemented(lv_event_t *e){
    lv_obj_t * mbox1 = lv_msgbox_create(NULL, 
    "Info", "Not yet implemented", NULL, true);
    lv_obj_center(mbox1);
}

/**
 * Create the Settings menu for configuring the board
*/
static void createWindowSettings(){
    // create a usable window to place components
    settingsWindow = createWindow("Settings");
    
    // content inside the window
    lv_obj_t * cont = lv_win_get_content(settingsWindow);


    lv_obj_t * list = lv_list_create(cont);
    // decide where to place the list
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);
    
    
    // add the setting items
    createListButton(list, LV_SYMBOL_GPS, "LoRa", event_settings_not_implemented);
    createListButton(list, LV_SYMBOL_WIFI, "WiFi", event_settings_wifi);
    createListButton(list, LV_SYMBOL_BLUETOOTH, "Bluetooth", event_settings_not_implemented);
    createListButton(list, LV_SYMBOL_SD_CARD, "Storage", event_settings_not_implemented);
    createListButton(list, LV_SYMBOL_BATTERY_FULL, "Power", event_settings_not_implemented);
    createListButton(list, LV_SYMBOL_USB, "Expansion ports", event_settings_not_implemented);
    createListButton(list, LV_SYMBOL_EYE_OPEN, "Language", event_settings_not_implemented);
}


/**
 * Settings click
*/
static void btn_event_settings(lv_event_t *e){
}


static void addEventButtonSettings(){
  lv_obj_add_event_cb(settingBtn, btn_event_settings, LV_EVENT_CLICKED, NULL);
}

/*
static void createSettingsButton(){
  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 15, 0);
  labelSettingsButton = lv_label_create(settingBtn); 
  lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS); 
  lv_obj_center(labelSettingsButton);
}
*/
