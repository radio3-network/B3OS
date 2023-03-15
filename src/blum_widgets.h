#include <Arduino.h>
#include <lvgl.h>

#ifndef BLUM_GLOBAL_H
#include "blum_global.h"
#endif

#ifndef BLUM_STATUS_BAR_H
#include "blum_status_bar.h"
#endif

#ifndef BLUM_WIDGETS_H



/*
    Create a button at specific coordinates
*/
static lv_obj_t* createButton(
    lv_coord_t x, lv_coord_t y,
    lv_coord_t length, lv_coord_t height, 
    const char* text, lv_event_cb_t event_toLaunch);


/**
 * Create a roller with multiple choices
*/
static lv_obj_t* createRoller(
    lv_coord_t x, lv_coord_t y,
    lv_coord_t length, lv_coord_t height,
    const char *items, lv_event_cb_t event_toLaunch
    );


/**
 * Specific click on the settings button to
 * display the settings dialog
*/
static void btn_event_keyboardKeypress(lv_event_t *e);

/**
 * Shows a keyboard on the screen.
 * This keyboard needs to be attached to
 * a target object where the typed text
 * will be visible. 
*/
static void keyboardShow(lv_obj_t *targetTextArea);

static void keyboardHide();


static void btn_event_togglekeyboard(lv_event_t *e);

/**
 * When clicking inside a text area, show the keyboard
*/
static void btn_event_textAreaClickedShowKeyboard(lv_event_t *e);


/**
 * Creates a text area at the given coordinates
*/
static lv_obj_t* createTextArea(lv_coord_t x, lv_coord_t y,
    lv_coord_t width, lv_coord_t height, const char *text);

#endif