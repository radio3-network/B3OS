#ifndef PreferencesArrayLibrary_h
#define PreferencesArrayLibrary_h

#include <ArduinoJson.h>
#include <Preferences.h>

/*

Syntax example:

#include "PreferencesArrayLibrary.h"

PreferencesArrayLibrary preferencesArray;

void setup() {
  preferencesArray.preferences.begin("my-app", false);
  // ...
}



  preferencesArray.preferences.begin("my-app", false);
  preferencesArray.addToArray("key1", "value1");
  preferencesArray.addToArray("key2", "value2");


  String value = preferencesArray.getFromArray("key1");
  if (value != "") {
    // Do something with the retrieved value
  }


*/

class PreferencesArrayLibrary
{
public:
    void addToArray(const char *key, const char *value);
    String getFromArray(const char *key);

private:
    Preferences preferences;
};

#endif
