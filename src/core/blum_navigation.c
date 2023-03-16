#include <Arduino.h>
#include <lvgl.h>
#include "blum_global.h"
#include "../utils/utils.h"

/*

    This file provides support for navigation trees.

    When the user clicks on the menus, this is the
    functionality that permits to "go back" between
    screens.

    It depends at an HashMap that includes all
    active screens, using the screen title as key.

    The HashMap is updated every time a new window
    is created at blum_widgets createWindow()
    

    // bring the screen forward
    lv_obj_move_foreground(settingsWindow);


    // update the icons
    static void statusBarBackButton(){
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_LEFT);
    }

    static void statusBarSettingsButton(){
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
    }

    // update the title
    statusBarTextUpdate(title);

static void statusBarTextUpdate(const char* text){
    lv_label_set_text(statusTextLabel, text );
}


*/





static int navGetIndex(){
    for(int i = 0; i < max_navigation_count; i++){
        if(strlen(indexData[i]) == 0) {
            return i;
        }
    }
    // already full
    return -1;
}

static void navDelete(int indexNumber){
    strcpy(indexData[indexNumber], "");
}

/**
 * The beginning. Starts with the home window
*/
static void navClean(){
    for(int i = 0; i < max_navigation_count; i++){
        // nullify all boxes
        strcpy(indexData[i], "");
    }
    // reset the icon for settings
    lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
}

static void navNew(const char *title){
    // get the index number
    int i = navGetIndex();
    if(i == -1){
        return;
    }

    char titleId[50];
    sprintf(titleId, "%s %d", title, i);

    // change the title on the status bar
    lv_label_set_text(statusTextLabel, titleId);
    // add the appropriate icon
    if(i == 0){
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
    }else{
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_LEFT);
    }
    // add to memory
    strcpy(indexData[i], title);
}

static void navGoBack(){
    // get the current index
    int i = navGetIndex();
    if(i == -1){
        return;
    }
    // delete the current index memory
    navDelete(i);
    //TODO delete the related window
    // get the window name
    const char * key = indexData[i];
    // using the window name, get the window object
    lv_obj_t * win = (lv_obj_t*) HashMapGet(mapWindows, key);
    // bring this window object to the foreground
    lv_obj_move_foreground(win);
}

