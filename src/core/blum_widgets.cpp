#include <Arduino.h>
#include <lvgl.h>
#include "utils/utils.h"
#include "blum_global.h"
#include "blum_status_bar.h"
#include "core/blum_navigation.h"


static lv_obj_t * createListButton(
  lv_obj_t * list, const void *icon,
  const char *txt, lv_event_cb_t event_cb){
  lv_obj_t * btn = lv_list_add_btn(list, icon, txt);
  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
  return btn;
}


static lv_obj_t* createWindow(const char *title){

    lv_obj_t* win;
    
    // avoid creating a new window when it already exists
    if(HashMapContainsKey(mapWindows, title)){
        // provide the previously created window
        lv_obj_t* existingWindow = (lv_obj_t*) HashMapGet(mapWindows, title);
        return existingWindow;        
    }else{
        // define a window with a 0 sized header (to hide it)
        win = lv_win_create(lv_scr_act(), 0);
        // add to the map
        HashMapInsert(mapWindows, title, (void*)win);
        // use all screen except the top because of the status bar
        lv_obj_set_size(win, LV_HOR_RES, LV_VER_RES - statusBarWeight);
        lv_obj_align(win, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_win_add_title(win, title);
    }

    // add to the index
    navNew(title);

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
 * Specific click on the settings button to
 * display the settings dialog
*/
static void btn_event_keyboardKeypress(lv_event_t *e)
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

    // raise the focus
    lv_group_t *group = lv_group_create();      // create a group
    lv_group_add_obj(group, targetTextArea);    // add the object to the group
    lv_group_add_obj(group, kb);                // add the object to the group
    lv_group_focus_obj(targetTextArea);         // set focus on the object

}

static void keyboardHide(){
    if(keyboardvisible == false){
        return;
    }
    // hide the keyboard
    lv_obj_del(kb);
    keyboardvisible = false;
}


static void btn_event_togglekeyboard(lv_event_t *e){
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
static void btn_event_textAreaClickedShowKeyboard(lv_event_t *e)
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
static lv_obj_t* createTextArea(lv_coord_t x, lv_coord_t y,
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

