
#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"



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



static void iconWifiCreate(){
  if(statusIconWifi != NULL){
    return;
  }
  statusIconWifi = lv_label_create(statusBar);
  lv_label_set_text(statusIconWifi, LV_SYMBOL_WIFI);
  lv_obj_align(statusIconWifi, LV_ALIGN_RIGHT_MID, -20, 0);
  Serial.println("Wifi: Turn ON");
  //lv_obj_invalidate(statusBar);
}

static void iconWifiRemove(){
  if(statusIconWifi == NULL){
    return;
  }
  lv_obj_clean(statusIconWifi);
  lv_label_set_text(statusIconWifi, LV_SYMBOL_DUMMY);
  lv_obj_del(statusIconWifi);
  statusIconWifi = NULL;
  Serial.println("Wifi: Turn OFF");
  //lv_obj_invalidate(statusBar);
}

/**
 * Create the status bar always present on top of the device
*/
static void buildStatusBar() {
  
    Serial.println("Build status bar");
    

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0xC5C5C5));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);

  statusBar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(statusBar, screen_width, 30);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, -1);
  lv_obj_remove_style(statusBar, NULL, LV_PART_SCROLLBAR | LV_STATE_ANY);

  // make sure the status bar does not move (scroll) elsewhere  
  lv_obj_set_scroll_dir(statusBar, LV_DIR_NONE);

  // status label
  statusTextLabel = lv_label_create(statusBar);
  lv_obj_set_size(statusTextLabel, screen_width - 50, 30);
  lv_label_set_text(statusTextLabel, " " );
  lv_obj_align(statusTextLabel, LV_ALIGN_LEFT_MID, 28, 7);

  // wifi icon
  if(isWiFiConnected()){
    iconWifiCreate();
  }

  // home button
  homeBtn = lv_btn_create(statusBar);
  lv_obj_set_size(homeBtn, 30, 30);
  lv_obj_align(homeBtn, LV_ALIGN_LEFT_MID, -15, 0);
  lv_obj_t *labelHome = lv_label_create(homeBtn); 
  lv_label_set_text(labelHome, LV_SYMBOL_HOME);  
  lv_obj_center(labelHome);

  // settings button
  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 15, 0);
  labelSettingsButton = lv_label_create(settingBtn); 
  lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS); 
  lv_obj_center(labelSettingsButton);
}
