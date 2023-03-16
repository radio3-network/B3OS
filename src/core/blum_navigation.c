#include <Arduino.h>
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



// max number of deep navigation
#define max_navigation_count 10

// array of 10 strings, each with up to 100 characters
static char indexData[max_navigation_count][100]; 
    


static int navGetIndex(){
    // when the first is null this means home
    if(indexData[0] == NULL){
        return 0;
    }
    for(int i = 1; i < max_navigation_count; i++){
        if(indexData[i] == NULL){
            return i-1;
        }
    }
    // already full
    return -1;
}

static void navDelete(int indexNumber){
    strcpy(indexData[indexNumber], NULL);
}

/**
 * The beginning. Starts with the home window
*/
static void navClean(){
    for(int i = 0; i < max_navigation_count; i++){
        // nullify all boxes
        strcpy(indexData[i], NULL);
    }
}

static void navNew(const char *title){
    int i = navGetIndex();
    if(i == -1){
        return;
    }
    strcpy(indexData[i], title);
}

static void navGoBack(){
    int i = navGetIndex();
    if(i == -1){
        return;
    }
    navDelete(i);
}

