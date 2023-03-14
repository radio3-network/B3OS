#include <Arduino.h>
#include <EEPROM.h>
#include <lvgl.h>
#include <esp32_smartdisplay.h>

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
+ code for automatic brightness change
+ internal RGB led effects (e.g. breathing)
+ switch between screen dark mode and daylight
+ app launcher
*/



// hardware-specific definitions

static int screen_width = 320;
static int screen_height = 240;


// LVGL Objects
static lv_obj_t *label_timer;
static lv_obj_t *label_some_text;

// global objects
static lv_obj_t *kb;
static boolean keyboardvisible = false;

void display_update()
{
    //char time_buffer[32];
    //sprintf(time_buffer, "%d", millis());
    //lv_label_set_text(label_timer, time_buffer);
}

void btn_event_cb(lv_event_t *e)
{
    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}


/**
 * Specific click on the settings button to
 * display the settings dialog
*/
void btn_event_settings(lv_event_t *e)
{
    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
       /* static uint8_t cnt = 0;
        cnt++;

        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
        */
    }
}


/*
    Create a button at specific coordinates
*/
static lv_obj_t* createButton(
    lv_coord_t x, lv_coord_t y,
    lv_coord_t length, lv_coord_t height, 
    const char* text, lv_event_cb_t event_toLaunch){
     // Create a buttom
    auto btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, length, height);
    lv_obj_add_event_cb(btn, event_toLaunch, LV_EVENT_ALL, NULL);
    // Set label to Button
    auto label = lv_label_create(btn);
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
    auto scr = lv_scr_act();
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
 * Shows a keyboard on the screen.
 * This keyboard needs to be attached to
 * a target object where the typed text
 * will be visible. 
*/
static void keyboardShow(lv_obj_t *targetTextArea, lv_event_cb_t event_toLaunch){
    kb = lv_keyboard_create(lv_scr_act());
    // use half of the screen available (expect 320x240 resolution)
    lv_obj_set_size(kb, LV_HOR_RES-2, LV_VER_RES / 2);
    //LV_ALIGN_CENTER
    lv_obj_align(kb, NULL, LV_DIR_RIGHT, LV_VER_RES / 2);

    lv_obj_add_event_cb(kb, event_toLaunch, LV_EVENT_ALL, NULL);
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
    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        if(keyboardvisible){
            keyboardHide();
        }else{
            keyboardShow(NULL, NULL);
        }
    }
}

/**
 * Creates a text area at the given coordinates
*/
static lv_obj_t* textAreaCreate(lv_coord_t x, lv_coord_t y,
    lv_coord_t width, lv_coord_t height, const char *text){
    auto scr = lv_scr_act();
    lv_obj_t *textArea = lv_textarea_create(scr);
    lv_obj_set_x(textArea, x);
    lv_obj_set_y(textArea, y);
    lv_obj_set_width(textArea, width);
    lv_obj_set_height(textArea, height);
    //lv_obj_set_align(textArea, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(textArea, text);
    //TODO when user clicks on text area, the keyboard must show up
    //TODO when the user clicks outside the text area, the keyboard goes away
    //TODO define this as an event for the text area 
    return textArea;
}




static lv_obj_t *settingBtn;
static lv_obj_t *statusTextLabel;

static void buildStatusBar() {

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0xC5C5C5));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);

  lv_obj_t *statusBar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(statusBar, screen_width, 30);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, -1);

  lv_obj_remove_style(statusBar, NULL, LV_PART_SCROLLBAR | LV_STATE_ANY);

  statusTextLabel = lv_label_create(statusBar);
  lv_obj_set_size(statusTextLabel, screen_width - 50, 30);

  lv_label_set_text(statusTextLabel, LV_SYMBOL_WIFI " WiFi Not Connected" );
  lv_obj_align(statusTextLabel, LV_ALIGN_LEFT_MID, 2, 7);

  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 15, 0);

  lv_obj_add_event_cb(settingBtn, btn_event_settings, LV_EVENT_ALL, NULL);
  lv_obj_t *label = lv_label_create(settingBtn); /*Add a label to the button*/
  lv_label_set_text(label, LV_SYMBOL_SETTINGS);  /*Set the labels text*/
  lv_obj_center(label);
}





static void buildLedColorLoop(){

    auto r = (byte)(millis() / 75);
    auto g = (byte)(millis() / 10);
    auto b = (byte)(millis() / 150);

    smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = b, .green = g, .red = r}}));

}


void setup_mainscreen()
{
    // Clear screen
    lv_obj_clean(lv_scr_act());

    // Create a button
    createButton(10, 10, 120, 50, "Teste", btn_event_cb);
    createButton(10, 70, 120, 50, "Keyboard", btn_event_togglekeyboard);
    
    const char* items = 
        "TESTE one\n"
        "TESTE two\n"
        "TESTE three\n"
        "TESTE four\n"
        "TEST five\n"
        "TEST six";

    //createRoller(140, 10, 160, 100, items, NULL);
    auto textArea = textAreaCreate(140, 10, 160, 100, "Write here..");

    keyboardShow(textArea, NULL);

    buildStatusBar();

/*
    label_timer = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_timer, &lv_font_montserrat_22, LV_STATE_DEFAULT);
    lv_obj_align(label_timer, LV_ALIGN_BOTTOM_MID, 0, -50);

    label_some_text = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_some_text, &lv_font_montserrat_22, LV_STATE_DEFAULT);
    lv_obj_align(label_some_text, LV_ALIGN_BOTTOM_MID, 0, -80);
    */
}

static  int64_t lastTime = esp_timer_get_time();

void setup()
{
    Serial.begin(115200);

    // initialize the hardware
    smartdisplay_init();
    setup_mainscreen();
}


void loop()
{
    // change the builtin led for fun
    buildLedColorLoop();

    display_update();
    lv_timer_handler();

    int64_t time = esp_timer_get_time();

    // show the current luminosity level
    if(time > (lastTime + 5000)){
        lastTime = esp_timer_get_time();
        String data = "Light value ";
        int value = smartdisplay_get_light_intensity();
        Serial.print(data);
        Serial.println(value);

        ///Serial.print("EEPROM size: ");
        //Serial.println(EEPROM.length());
    }


    
    
}