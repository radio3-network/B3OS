#include "PreferencesArrayLibrary.h"

void PreferencesArrayLibrary::addToArray(const char* key, const char* value) {
  // Retrieve the existing JSON string from Preferences
  String json = preferences.getString("settings");

  // Parse the JSON string into a JSON object using ArduinoJson
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  // Add a new key-value pair to the JSON object
  JsonObject obj = doc.createNestedObject();
  obj[key] = value;

  // Serialize the updated JSON object back into a string
  String updatedJson;
  serializeJson(doc, updatedJson);

  // Store the updated JSON string back in Preferences
  preferences.putString("settings", updatedJson);
}

String PreferencesArrayLibrary::getFromArray(const char* key) {
  // Retrieve the existing JSON string from Preferences
  String json = preferences.getString("settings");

  // Parse the JSON string into a JSON object using ArduinoJson
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  // Find the object in the array with the given key
  for (JsonObject obj : doc.as<JsonArray>()) {
    if (obj.containsKey(key)) {
      return obj[key].as<String>();
    }
  }

  // Return an empty string if the key is not found
  return "";
}
