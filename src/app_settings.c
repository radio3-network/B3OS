#include <lvgl.h>

lv_obj_t* windowSettings1(void)
{
    /*
    // remove any previous window
    if(settingsWindow != NULL){
        lv_obj_del(settingsWindow);
    }
    */
    // create a usable window to place components
    lv_obj_t* settingsWindow = lv_win_create(lv_scr_act(), 0);
    // use all screen except the top because of the status bar
    lv_obj_set_size(settingsWindow, LV_HOR_RES, LV_VER_RES- 30);
    lv_obj_align(settingsWindow, LV_ALIGN_BOTTOM_MID, 0, 0);

    /*
    // define a window with a 0 sized header (to hide it)
    settingsWindow = lv_win_create(lv_scr_act(), 0);
    // use all screen except the top because of the status bar
    lv_obj_set_size(settingsWindow, LV_HOR_RES, LV_VER_RES- statusBarWeight);
    lv_obj_align(settingsWindow, LV_ALIGN_BOTTOM_MID, 0, 0);
    */
    // 

    lv_obj_t * btn;
    btn = lv_win_add_btn(settingsWindow, LV_SYMBOL_LEFT, 40);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_win_add_title(settingsWindow, "A title");

    btn = lv_win_add_btn(settingsWindow, LV_SYMBOL_RIGHT, 40);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_win_add_btn(settingsWindow, LV_SYMBOL_CLOSE, 60);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    // content inside the window
    lv_obj_t * cont = lv_win_get_content(settingsWindow);  /*Content can be aded here*/
    lv_obj_t * label =  lv_label_create(cont);
    lv_label_set_text(label, "This is\n"
                             "a pretty\n"
                             "long text\n"
                             "to see how\n"
                             "the window\n"
                             "becomes\n"
                             "scrollable.\n"
                             "\n"
                             "\n"
                             "Some more\n"
                             "text to be\n"
                             "sure it\n"
                             "overflows. :)");

    return settingsWindow;
}