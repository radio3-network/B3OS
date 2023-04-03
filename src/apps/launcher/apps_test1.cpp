#include <LittleFS.h>

void writeStringToFile(const char* fileName, const char* data) {
  // Open the file for writing in text mode
  File file = LittleFS.open(fileName, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Write the string to the file
  file.print(data);
   
    Serial.println("String written to file");
    
    Serial.print("Size: ");
    Serial.println(file.size());

    Serial.print("Offset: ");
    Serial.println(file.position());


  // Close the file
  file.close();

 
}