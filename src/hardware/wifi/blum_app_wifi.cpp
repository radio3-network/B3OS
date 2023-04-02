#include <string.h>
#include <Arduino.h>
#include <lvgl.h>
#include "core/blum_global.h"
#include "core/blum_widgets.h"

static bool scan_done = false;

static lv_obj_t *appWifiSwitch = NULL;

static String selectedSSID;

#define DEFAULT_SCAN_LIST_SIZE 30

/**
 *  Someone clicks on the "enable wifi button"
 */
static void btn_event_switch(lv_event_t *e) {
    wifiEnabled = lv_obj_has_state(appWifiSwitch, LV_STATE_CHECKED);

    // write to flash memory
    preferences.begin(NAMESPACE_GENERIC, false);
    preferences.putBool(KEY_WIFI_ENABLED, wifiEnabled);
    preferences.end();

    // start the wifi
    wifiStart();
}

/*
  Accept the new password being given
*/
static void ta_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        if (kb != NULL)
            lv_keyboard_set_textarea(kb, ta);
        return;
    }

    if (code != LV_EVENT_READY) {
        return;
    }

    // password was typed
    Serial.print("Password was written: ");
    Serial.println(lv_textarea_get_text(ta));

    // save the password
    preferences.begin(NAMESPACE_GENERIC, false);
    preferences.putString(KEY_WIFI_SSID, selectedSSID);
    preferences.putString(KEY_WIFI_PASSWORD, lv_textarea_get_text(ta));
    preferences.end();

    // start the wifi
    boolean result = wifiStart();

    // did it worked?
    if (result == false) {
        preferences.begin(NAMESPACE_GENERIC, false);
        preferences.putString(KEY_WIFI_PASSWORD, "");
        preferences.end();
        return;
    }
    // hide the keyboard
    keyboardHide();
    navGoBack();
}

static void btn_event_wifi_select(lv_event_t *e) {
    const char *name_c = (const char *)lv_event_get_user_data(e);
    String name = String(name_c);
    free((void *)name_c);

    Serial.print("Clicked to select Wifi: ");
    Serial.println(name);
    selectedSSID = name;

    // ask for a password
    inputWindow = createWindow("Password");
    // content inside the window
    lv_obj_t *cont = lv_win_get_content(inputWindow);

    /*Create the password box*/
    lv_obj_t *pwd_ta = lv_textarea_create(lv_scr_act());
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_obj_set_width(pwd_ta, lv_pct(90));
    lv_obj_add_event_cb(pwd_ta, ta_event_cb, LV_EVENT_ALL, NULL);

    // Create a keyboard
    keyboardShow(pwd_ta);

    /*kb = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(kb, LV_HOR_RES, LV_VER_RES / 2);
    lv_keyboard_set_textarea(kb, pwd_ta); //Focus it on one of the text areas to start
    // align on top of the keyboard
    lv_obj_align_to(pwd_ta, kb, LV_ALIGN_OUT_TOP_MID, 0, -20);
    

    // raise the focus
    lv_group_t *group = lv_group_create();  // create a group
    lv_group_add_obj(group, pwd_ta);        // add the object to the group
    lv_group_add_obj(group, kb);            // add the object to the group
    lv_group_focus_obj(pwd_ta);             // set focus on the object
    */
}

/**
 * dialog box for wifi
 */
static void createWindowWifi() {
    // create a new window
    wifiWindow = createWindow("WiFi");
    // content inside the window
    lv_obj_t *cont = lv_win_get_content(wifiWindow);

    // place a switch to use wifi or to shut it down
    appWifiSwitch = lv_switch_create(cont);

    // setup the initial state
    if (wifiEnabled) {
        lv_obj_add_state(appWifiSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(appWifiSwitch, LV_STATE_DEFAULT);
    }

    // add an event to when the switch is clicked
    lv_obj_add_event_cb(appWifiSwitch, btn_event_switch, LV_EVENT_CLICKED, NULL);

    // add a label to explain what the switch does
    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, "Use Wifi");
    lv_obj_align(label, LV_ALIGN_DEFAULT, 60, 7);

    // list the currently available wifi networks
    lv_obj_t *appWifiList = lv_list_create(cont);
    // decide where to place the list
    int margin = 40;
    lv_obj_set_size(appWifiList, lv_pct(100), lv_pct(75));
    lv_obj_align(appWifiList, LV_ALIGN_DEFAULT, 0, margin);

    // list all available wifi networks
    // TODO this only works on ESP devices
    // TODO this blocks the whole processing, add it as a thread

    Serial.println("WiFi scan starting..");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("WiFi scan done");
    if (n == 0) {
        Serial.println("No WiFi networks found");
        return;
    }

    // there are some networks found, list them
    for (int i = 0; i < n; ++i) {
        String name = WiFi.SSID(i);

        char *name_copy = (char *)malloc(name.length() + 1);
        strcpy(name_copy, name.c_str());

        int32_t rssi = WiFi.RSSI(i);

        // RSSI is always a negative number
        rssi = 100 + rssi;

        // convert the RSSI to a percentage
        // the max is 55 so we convert proportionally to a percentage
        // 55 --> 100
        //  x --> y
        int percentage = (rssi * 100) / 55;
        // when placed on top of the router this goes above 100%
        if (percentage > 100) {
            percentage = 100;
        }
        // create the text for the list item
        char output_text[50];
        sprintf(output_text, "%s (%d%%)", name_copy, percentage);
        // add the list item
        lv_obj_t *btn = lv_list_add_btn(appWifiList, LV_SYMBOL_WIFI, output_text);

        // add event to select a wifi network
        lv_obj_add_event_cb(btn, btn_event_wifi_select, LV_EVENT_CLICKED, (void *)name_copy);
    }
}

/*
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
*/
// the selected wifi network is on the top

static void event_settings_wifi(lv_event_t *e) {
    createWindowWifi();
}