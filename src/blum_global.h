#ifndef BLUM_GLOBAL_H
#define BLUM_GLOBAL_H

#include <lvgl.h>
#include <utils/utils.h>

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

static boolean enabledWifi = false;


// hardware-specific definitions
static int screen_width = 320;
static int screen_height = 240;


// internal settings
static int statusBarWeight = 30;


#endif