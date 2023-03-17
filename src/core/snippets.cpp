/*

Simple snippets for every day programming


    char titleId[50];
    sprintf(titleId, "%s %d", key, i);
    lv_label_set_text(statusTextLabel, titleId);

    const char * key = StringArray_get(indexData, i);

    lv_label_set_text(statusTextLabel, key);


    // get the state of the switch from EEPROM memory
  if (wifi_enabled != nullptr) {
    if(strcmp(wifi_enabled, "ON") == 0){
      lv_obj_add_state(sw, LV_STATE_CHECKED);
    }else{
      lv_obj_add_state(sw, LV_STATE_DEFAULT);
    }
  }

  // add an event
  lv_obj_add_event_cb(homeBtn, btn_event_home, LV_EVENT_CLICKED, NULL);

    static void btn_event_home(lv_event_t *e){
   
    }



*/