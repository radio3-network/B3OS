#include "esp_wifi.h"
#include <string.h>
#include <Arduino.h>
#include <lvgl.h>
#include "core/blum_global.h"
#include "core/blum_widgets.h"

// this WiFi access is specific to ESP-based WiFi boards
// example docs at https://github.com/espressif/esp-idf/tree/master/examples/wifi

static bool scan_done = false;

#define DEFAULT_SCAN_LIST_SIZE 30

static wifi_ap_record_t getWifiNetworks(){
    // based on https://github.com/espressif/esp-idf/blob/master/examples/wifi/scan/main/scan.c
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
        //const char* id = ap_info[i].ssid;
        //uint8_t rssi = ap_info[i].rssi;
        //wifi_auth_mode_t authMode = ap_info[i].authmode;
    }

    esp_wifi_stop;
    return *ap_info;
}

static void btn_event_switch(lv_event_t *e){
  enabledWifi = lv_obj_has_state(switchWifi, LV_STATE_CHECKED);

  // write to flash memory
  preferences.begin(NAMESPACE, false);
  preferences.putBool(KEY_WIFI_ENABLED, enabledWifi);
  preferences.end();

  // send a debug message
  if(debug){
   if(enabledWifi){
      iconWifiCreate();
      Serial.println("Wifi: Turn ON");
   }else{
      iconWifiRemove();
      Serial.println("Wifi: Turn OFF");
   }
   }
   
}


static void btn_event_wifi_select(lv_event_t *e){
     lv_obj_t *btn = lv_event_get_target(e);
     const char *output_text = static_cast<const char*>(lv_obj_get_user_data(btn));
     Serial.print("Clicked to select Wifi: ");
     Serial.println(output_text);
}


/**
 * dialog box for wifi
*/
static void createWindowWifi(){
  // create a new window
  wifiWindow = createWindow("WiFi");
  // content inside the window
  lv_obj_t * cont = lv_win_get_content(wifiWindow);

  // place a switch to use wifi or to shut it down
  switchWifi = lv_switch_create(cont);

  // setup the initial state
  if(enabledWifi){
    lv_obj_add_state(switchWifi, LV_STATE_CHECKED);
  }else{
    lv_obj_add_state(switchWifi, LV_STATE_DEFAULT);
  }

  // add an event to when the switch is clicked
  lv_obj_add_event_cb(switchWifi, btn_event_switch, LV_EVENT_CLICKED, NULL);
  
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
        // add event to select a wifi network
        lv_obj_add_event_cb(btn, btn_event_wifi_select, LV_EVENT_CLICKED, output_text);
    }

    esp_wifi_stop;

  // the selected wifi network is on the top


}

static void event_settings_wifi(lv_event_t *e){
  createWindowWifi();
}