#include <Arduino.h>
#include <lvgl.h>
//#include "app_settings.c"


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



// hardware-specific definitions


static int screen_width = 320;
static int screen_height = 240;


// internal settings
static int statusBarWeight = 30;

// global objects
static lv_obj_t *kb;
static boolean keyboardvisible = false;
static lv_obj_t *targetTextArea = NULL;
static lv_obj_t *homeBtn;
static lv_obj_t *statusTextLabel;
static lv_obj_t *statusIconWifi;

static lv_obj_t *settingBtn;
static lv_obj_t *settingsWindow;


/**
 * Specific click on the settings button to
 * display the settings dialog
*/
void btn_event_settings(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
       /* static uint8_t cnt = 0;
        cnt++;

        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
        */
    }
}



static lv_obj_t* createWindow(){
    // define a window with a 0 sized header (to hide it)
    lv_obj_t* win = lv_win_create(lv_scr_act(), 0);
    // use all screen except the top because of the status bar
    lv_obj_set_size(settingsWindow, LV_HOR_RES, LV_VER_RES- statusBarWeight);
    lv_obj_align(settingsWindow, LV_ALIGN_BOTTOM_MID, 0, 0);
    return win;
}


/*
    Create a button at specific coordinates
*/
static lv_obj_t* createButton(
    lv_coord_t x, lv_coord_t y,
    lv_coord_t length, lv_coord_t height, 
    const char* text, lv_event_cb_t event_toLaunch){
     // Create a buttom
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, length, height);
    lv_obj_add_event_cb(btn, event_toLaunch, LV_EVENT_ALL, NULL);
    // Set label to Button
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn;
}

/**
 * Create a roller with multiple choices
*/
static lv_obj_t* createRoller(
    lv_coord_t x, lv_coord_t y,
    lv_coord_t length, lv_coord_t height,
    const char *items, lv_event_cb_t event_toLaunch
    ){
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t* roller = lv_roller_create(scr);
    //lv_obj_set_layout(roller, LV_LAYOUT_FLEX);
    lv_obj_set_pos(roller, x, y);
    lv_obj_set_size(roller, length, height);
    //lv_roller_set_visible_row_count(roller, 4);
    lv_roller_set_options(roller,
        items,
        LV_ROLLER_MODE_NORMAL);
    //lv_roller_set_selected(roller, 0, LV_ANIM_ON);
    // associate an event when clicking the items
    if(event_toLaunch != NULL){
        lv_obj_add_event_cb(roller, event_toLaunch, LV_EVENT_ALL, NULL);
    }
    return roller;
}




/**
 * Update the status bar text
*/
void statusBarTextUpdate(const char* text){
    lv_label_set_text(statusTextLabel, text );
}

/**
 * Specific click on the settings button to
 * display the settings dialog
*/
void btn_event_keyboardKeypress(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        //lv_kb_def_event_cb(kb, event);  // call the default event handler
        //printf("Key pressed");//: %c\n", (char)(*key));  // print the key value as a character
        char time_buffer[14];
        sprintf(time_buffer, "%d", millis());
        statusBarTextUpdate(time_buffer);
        return;
    }

    if (code == LV_EVENT_CANCEL) {
        statusBarTextUpdate("Cancel");
        return;
    }



    if (code == LV_EVENT_CLICKED)
    {
        printf("Key pressed2");
       // log("Key pressed", 1);
       // log("code", code);
       // log("button", btn);

    }
}

/**
 * Shows a keyboard on the screen.
 * This keyboard needs to be attached to
 * a target object where the typed text
 * will be visible. 
*/
static void keyboardShow(lv_obj_t *targetTextArea){
    kb = lv_keyboard_create(lv_scr_act());
    // use half of the screen available (expect 320x240 resolution)
    lv_obj_set_size(kb, LV_HOR_RES-2, LV_VER_RES / 2);
    //LV_ALIGN_CENTER
    //TODO: this align default might break keyboard position
    lv_obj_align(kb, LV_ALIGN_DEFAULT, LV_DIR_RIGHT, LV_VER_RES / 2);

    // remove the event in case it had been added previously
    lv_obj_remove_event_cb(kb, btn_event_keyboardKeypress);
    lv_obj_add_event_cb(kb, btn_event_keyboardKeypress, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(kb, targetTextArea);
    keyboardvisible = true;
}

static void keyboardHide(){
    if(keyboardvisible == false){
        return;
    }
    // hide the keyboard
    lv_obj_del(kb);
    keyboardvisible = false;
}


void btn_event_togglekeyboard(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        if(keyboardvisible){
            keyboardHide();
        }else{
            keyboardShow(targetTextArea);
        }
    }
}

/**
 * When clicking inside a text area, show the keyboard
*/
void btn_event_textAreaClickedShowKeyboard(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
       if(keyboardvisible == false){
            keyboardShow(targetTextArea);
       }
    }
}


/**
 * Creates a text area at the given coordinates
*/
static lv_obj_t* textAreaCreate(lv_coord_t x, lv_coord_t y,
    lv_coord_t width, lv_coord_t height, const char *text){
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *textArea = lv_textarea_create(scr);
    lv_obj_set_x(textArea, x);
    lv_obj_set_y(textArea, y);
    lv_obj_set_width(textArea, width);
    lv_obj_set_height(textArea, height);
    lv_textarea_set_placeholder_text(textArea, text);
    targetTextArea = textArea;
    //TODO when user clicks on text area, the keyboard must show up
    lv_obj_add_event_cb(textArea, btn_event_textAreaClickedShowKeyboard, LV_EVENT_ALL, NULL);
    //TODO when the user clicks outside the text area, the keyboard goes away
    //TODO define this as an event for the text area 
    return textArea;
}



void buildStatusBar() {


  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0xC5C5C5));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);

  lv_obj_t *statusBar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(statusBar, screen_width, 30);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, -1);

  lv_obj_remove_style(statusBar, NULL, LV_PART_SCROLLBAR | LV_STATE_ANY);

  // status lavel
  statusTextLabel = lv_label_create(statusBar);
  lv_obj_set_size(statusTextLabel, screen_width - 50, 30);
  lv_label_set_text(statusTextLabel, " " );
  lv_obj_align(statusTextLabel, LV_ALIGN_LEFT_MID, 32, 7);

  // wifi icon
  statusIconWifi = lv_label_create(statusBar);
  lv_label_set_text(statusIconWifi, LV_SYMBOL_WIFI);
  lv_obj_align(statusIconWifi, LV_ALIGN_RIGHT_MID, -20, 0);
  

  // home button
  homeBtn = lv_btn_create(statusBar);
  lv_obj_set_size(homeBtn, 30, 30);
  lv_obj_align(homeBtn, LV_ALIGN_LEFT_MID, -15, 0);

  lv_obj_add_event_cb(homeBtn, btn_event_settings, LV_EVENT_ALL, NULL);
  lv_obj_t *labelHome = lv_label_create(homeBtn); 
  lv_label_set_text(labelHome, LV_SYMBOL_HOME);  
  lv_obj_center(labelHome);

  // settings button
  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 15, 0);

  lv_obj_add_event_cb(settingBtn, btn_event_settings, LV_EVENT_ALL, NULL);
  lv_obj_t *label = lv_label_create(settingBtn); 
  lv_label_set_text(label, LV_SYMBOL_SETTINGS); 
  lv_obj_center(label);
  
}

