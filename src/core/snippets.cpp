/*

Simple snippets for every day programming


    char titleId[50];
    sprintf(titleId, "%s %d", key, i);
    lv_label_set_text(statusTextLabel, titleId);

    const char * key = StringArray_get(indexData, i);

    lv_label_set_text(statusTextLabel, key);



  // convert a char to string and compare the first character
  char* args = "/test"; // example command line argument
  String argString = String(args); // convert char* to String
  if (argString.startsWith("/")) { // check if string starts with "/"
    Serial.println("The argument starts with '/'");
  } else {
    Serial.println("The argument does not start with '/'");
  }



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