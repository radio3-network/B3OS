#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"
#include "blum_widgets.h"
#include "blum_status_bar.h"
#include "blum_app_home.h"
#include "blum_app_settings.h"
#include "blum_app_wifi.h"

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


TODO:
+ define app content area
++ respects status bar and keyboard resizing
+ clicking on textArea shows keyboard
++ clicking away from textArea hides keyboard
++ pressing keyboard button hides/toggles keyboard
+ dialog for wifi selection/save/connect
x status bar with icons (e.g. WIFI)
xx include builtin icons
+ connectivity icons on status bar
+ bluetooth, wifi, lora
+ add generic icon functionlity
++ define PNG source on SD drive
++ launch an app/script or method
++ support dark and daylight mode
+ dialog for brightness (e.g. manual/automatic)
x code for automatic brightness change
x internal RGB led effects (e.g. breathing)
+ switch between screen dark mode and daylight
+ app launcher
*/






static void createWindowHome(){
  // remove any previous window
    if(homeWindow != NULL){
        lv_obj_del(homeWindow);
    }
    
    // create a usable window to place components
    homeWindow = createWindow("Home");
    // reset settings for the start
    previousWindow = NULL;
    // set the status bar button ready for pressing
    statusBarSettingsButton();
}

/**
 * Home click
*/
void btn_event_home(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED){
       createWindowHome();
    }
}

static lv_obj_t * createListButton(
  lv_obj_t * list, const void *icon,
  const char *txt, lv_event_cb_t event_cb){
  lv_obj_t * btn = lv_list_add_btn(list, icon, txt);
  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
  return btn;
}



/**
 * dialog box for wifi
*/
void createWindowWifi(){
  if(wifiWindow != NULL){
    lv_obj_move_foreground(wifiWindow);
    currentWindow = wifiWindow;
    statusBarBackButton();
    return;
  }else{
    // create a new window
    wifiWindow = createWindow("WiFi");
  }
  // content inside the window
  lv_obj_t * cont = lv_win_get_content(wifiWindow);

  // place a switch to use wifi or to shut it down
  lv_obj_t * sw = lv_switch_create(cont);
  lv_obj_add_state(sw, LV_STATE_DEFAULT);
  // add a label to explain what the switch does
  lv_obj_t * label = lv_label_create(cont);
  lv_label_set_text(label, "Use Wifi" );
  lv_obj_align(label, LV_ALIGN_DEFAULT, 60, 7);
  
  
  // list the currently available wifi networks
  lv_obj_t * list = lv_list_create(cont);
  // decide where to place the list
  int margin = 40;
  lv_obj_set_size(list, lv_pct(100), lv_pct(75));
  lv_obj_align(list, LV_ALIGN_DEFAULT, 0, margin);

  // list all available wifi networks
  // TODO this only works on ESP devices
  // TODO this blocks the whole processing, add it as a thread
  
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_records(&number, ap_info);
    esp_wifi_scan_get_ap_num(&ap_count);

    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
        char ssid_text[33];
        memcpy(ssid_text, ap_info[i].ssid, 33);
        int rssi = 100 + ap_info[i].rssi;
        // 55 RSSI is the maximum reference value
        int percentage = (rssi * 100) / 55;
        // when placed on top of the router this goes above 100%
        if(percentage > 100){
          percentage = 100;
        }
        char output_text[50];
        sprintf(output_text, "%s (%d%%)", ssid_text, percentage);

        lv_obj_t * btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, output_text);
        //TODO: add event to select a wifi network
    }

    esp_wifi_stop;

  // the selected wifi network is on the top


}

void event_settings_wifi(lv_event_t *e){
  createWindowWifi();
}


/**
 * Create the Settings menu for configuring the board
*/
void createWindowSettings(){
    if(settingsWindow != NULL){
      lv_obj_move_foreground(settingsWindow);
      statusBarBackButton();
      currentWindow = settingsWindow;
      previousWindow = NULL;
      return;
    }else{
      // create a usable window to place components
      settingsWindow = createWindow("Settings");
    }
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
void btn_event_settings(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    
    if(currentWindow == settingsWindow){
      statusBarSettingsButton();
      lv_obj_move_foreground(homeWindow);
      currentWindow = homeWindow;
      previousWindow = NULL;
      return;
    }


    if(previousWindow == NULL){
      if(settingsWindow == NULL){
            createWindowSettings();
            return;
        }else{
            lv_obj_move_foreground(settingsWindow);
            return;
        }
    }

      lv_obj_move_foreground(previousWindow);
      currentWindow = previousWindow;
      previousWindow = NULL;
}



static void createSettingsButton(){
  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 15, 0);
  lv_obj_add_event_cb(settingBtn, btn_event_settings, LV_EVENT_CLICKED, NULL);
  labelSettingsButton = lv_label_create(settingBtn); 
  lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS); 
  lv_obj_center(labelSettingsButton);
}







/**
 * Create the status bar always present on top of the device
*/
static void buildStatusBar() {

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
  statusIconWifi = lv_label_create(statusBar);
  lv_label_set_text(statusIconWifi, LV_SYMBOL_WIFI);
  lv_obj_align(statusIconWifi, LV_ALIGN_RIGHT_MID, -20, 0);
  

  // home button
  homeBtn = lv_btn_create(statusBar);
  lv_obj_set_size(homeBtn, 30, 30);
  lv_obj_align(homeBtn, LV_ALIGN_LEFT_MID, -15, 0);
  lv_obj_add_event_cb(homeBtn, btn_event_home, LV_EVENT_ALL, NULL);
  lv_obj_t *labelHome = lv_label_create(homeBtn); 
  lv_label_set_text(labelHome, LV_SYMBOL_HOME);  
  lv_obj_center(labelHome);

  // settings button
  createSettingsButton();
}



static void start(){
    // Clear screen
    lv_obj_clean(lv_scr_act());
    buildStatusBar();
    //tileView();
    createWindowHome();
}

