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

static int navGetNextFreeIndex() {
    return StringArray_size(indexData);
}

static void navDelete(int indexNumber) {
    StringArray_delete(indexData, indexNumber);
}

/**
 * The beginning. Starts with the home window
 */
static void navClean() {
    if (indexData != NULL) {
        int i = StringArray_size(indexData) - 1;
        /*if (i > 0) {
            const char *windowPresent = StringArray_get(indexData, i);
            lv_obj_t *winPresent = (lv_obj_t *)HashMapGet(mapWindows, windowPresent);
            lv_obj_clean(winPresent);
            lv_obj_del(winPresent);
        }*/
        StringArray_destroy(indexData);
    }
    // create a new index
    indexData = StringArray_create(max_navigation_count);
    // reset the icon for settings
    lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
}

static void navNew(const char *title) {
    // add it to memory
    StringArray_add(indexData, title);

    int i = StringArray_size(indexData) - 1;
    const char *key = StringArray_get(indexData, i);

    // change the title on the status bar
    lv_label_set_text(statusTextLabel, title);
    // add the appropriate icon
    if (i == 0) {
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_SETTINGS);
    } else {
        lv_label_set_text(labelSettingsButton, LV_SYMBOL_LEFT);
    }
}

static void navGoBack() {
    int i = StringArray_size(indexData) - 2;
    const char *windowFuture = StringArray_get(indexData, i);
    const char *windowPresent = StringArray_get(indexData, i + 1);

    // using the window name, get the window object
    lv_obj_t *winFuture = (lv_obj_t *)HashMapGet(mapWindows, windowFuture);
    lv_obj_t *winPresent = (lv_obj_t *)HashMapGet(mapWindows, windowPresent);

    // remove all old items
    //lv_obj_move_background(winPresent);
    lv_obj_clean(winPresent);
    lv_obj_del(winPresent);

    // bring this window object to the foreground
    lv_obj_move_foreground(winFuture);

    lv_task_handler();
    // update the label
    lv_label_set_text(statusTextLabel, windowFuture);

    // means we are on the root again
    if (i == 0) {
        // delete the memory map
        HashMapClear(mapWindows);
        // add again the root window
        HashMapInsert(mapWindows, windowFuture, winFuture);
        // restore the icon and index
        navClean();
        // add home again on the index
        navNew(windowFuture);
        // stop it here
        return;
    }

    // delete the current index memory
    navDelete(i + 1);
}
