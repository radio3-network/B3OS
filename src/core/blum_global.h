#ifndef BLUM_GLOBAL_H
#define BLUM_GLOBAL_H

#include <lvgl.h>
#include <Preferences.h>
#include <utils/utils.h>
#include "hardware/hardware.h"
#include "apps/terminal/terminal.hpp"


// global settings
static bool debug = true;


// global objects
static lv_obj_t *kb;
static boolean keyboardvisible = false;
static lv_obj_t *targetTextArea = NULL;
static lv_obj_t *statusBar = NULL;
static lv_obj_t *statusTextLabel;
static lv_obj_t *statusIconWifi;
static lv_obj_t * wifiWindow;

static lv_obj_t *labelSettingsButton;
static lv_obj_t *settingBtn;
static lv_obj_t *settingsWindow = NULL;

static lv_obj_t *homeBtn;
static lv_obj_t *homeWindow = NULL;

static lv_obj_t *previousBtn;
static HashMap* mapWindows = NULL;

// max number of deep navigation
#define max_navigation_count 10

// navigation index
static StringArray *indexData;


// settings saved on flash

static Preferences preferences;
static const char* NAMESPACE_GENERIC = "internal";

// globals for hardware modules
static const char*  KEY_WIFI_ENABLED = "WIFI_ENABLED";
static const char*  KEY_WIFI_SSID = "WIFI_SSID";
static const char*  KEY_WIFI_PASSWORD = "WIFI_PASSWORD";

static String wifi_ssid = "";
static String wifi_password = "";
#define SERVER_PORT 23
  

static boolean wifiEnabled = false;


static lv_obj_t * switchWifi = NULL;


// hardware-specific definitions
static int screen_width = 320;
static int screen_height = 240;


// internal settings
static int statusBarWeight = 30;


#include "blum_global.c"

#endif