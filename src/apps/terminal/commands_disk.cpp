#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include <hardware/hardware.h>
#include "commands_disk.hpp"
#include "terminal.hpp"

// Storage card
#include "SdFat.h"
#include "sdios.h"

uint32_t cardSectorCount = 0;
uint8_t  sectorBuffer[512];
// SdCardFactory constructs and initializes the appropriate card.
SdCardFactory cardFactory;
// Pointer to generic SD card.
SdCard* m_card = nullptr;

#define SPI_CLOCK SD_SCK_MHZ(6)
#define SD_CONFIG SdSpiConfig(TF_PIN_CS, DEDICATED_SPI, SPI_CLOCK)

String currentPath = "/";
SdFat sd;




void card_initialize(){
  // Select and initialize proper card driver.
  m_card = cardFactory.newCard(SD_CONFIG);
  if (!m_card || m_card->errorCode()) {
    Serial.println("Error initializing card factory");
    return;
  }

  cardSectorCount = m_card->sectorCount();
  if (!cardSectorCount) {
    Serial.println("Get sector count failed.");
    return;
  }

   if (!sd.begin(SD_CONFIG)) {
    Serial.println("Error initializing storage card.");
  }

}

// format a TF/SD card available on a single slot
void func_formatCard(char *args, Stream *response ){
  
  ExFatFormatter exFatFormatter;
  FatFormatter fatFormatter;

  // Format exFAT if larger than 32GB.
  bool rtn = cardSectorCount > 67108864 ?
  exFatFormatter.format(m_card, sectorBuffer, &Serial) :
  fatFormatter.format(m_card, sectorBuffer, &Serial);

  if (!rtn) {
    response -> println("Error formatting storage card.");
    return;
  }

  response -> println("Storage card formatted successfully.");
}


/**
 * Returns the canonical path for a char with a file name
 */
String getPath(char *args){
  // convert args to String
  String argString = String(args);
  // canonical path
  if(argString.startsWith("/")){
    return argString;
  }else{
    return currentPath + argString;    
  }
}



void func_touch(char *args, Stream *response) {
  if (args == NULL) {
    response->println("No file name specified");
    return;
  }

  String path = getPath(args);
  // open file in write mode
  File32 file = sd.open(path, FILE_WRITE);
  if (!file) {
    response->println("Failed to create file: " + path);
    return;
  }

  file.close();
  response->println("File created");
}


void func_mkdir(char *args, Stream *response ){
  String path = getPath(args);
  if (!sd.mkdir(path)) {
    response -> println("Failed to create folder");
  }
}

void func_rm(char *args, Stream *response ){
  if (args == NULL) {
    response->println("No file or folder specified");
    return;
  }
  String path = getPath(args);
  if (!sd.remove(path)) {
    response -> println("Failed to remove file");
  }
}


// Helper function to zero-pad a number to two digits
String zeroPad(int n) {
  if (n < 10) {
    return "0" + String(n);
  } else {
    return String(n);
  }
}


String getModifyDateTime(FatFile file) {
  uint16_t date, time;
  if (!file.getModifyDateTime(&date, &time)) {
    return ""; // Return empty string on error
  }

  // Extract year, month, and day from the date
  uint16_t year = ((date >> 9) & 0x7F) + 1980;
  uint8_t month = (date >> 5) & 0x0F;
  uint8_t day = date & 0x1F;

  // Extract hour, minute, and second from the time
  uint8_t hour = (time >> 11) & 0x1F;
  uint8_t minute = (time >> 5) & 0x3F;
  uint8_t second = (time & 0x1F) * 2;

  // Format the date and time as a string
  return String(year) + "-" + zeroPad(month) + "-" + zeroPad(day) + " " +
         zeroPad(hour) + ":" + zeroPad(minute) + ":" + zeroPad(second);
}



// prints a line of text
void func_ll(char *args, Stream *response ){
  File32 dir;
  File32 file;

  // open the current directory
  if (!dir.openCwd()){
    response -> println("Failed to open the directory");
    return;
  }

  while (file.openNext(&dir, O_RDONLY)) {
    char fileName[255];
    size_t len = file.getName8(fileName, sizeof(fileName));


    String time = getModifyDateTime(file);
    response -> print(time + " ");

    if(file.isDirectory()){
      // 34 == blue, 37 = white, 32 = green
      shell.setTerminalCharacterColor(response, 1, 34);
      response -> print(fileName);
      response -> print("/");
      shell.setTerminalCharacterColor(response, 0, 37);
      response -> print("\r\n");
    }else{
      response -> print(fileName);
      response -> print("\r\n");
    }
    file.close();
  }
  dir.close();
}

// prints a line of text
void func_ls(char *args, Stream *response ){
  File32 dir;
  File32 file;

  // open the current directory
  if (!dir.openCwd()){
    response -> println("Failed to open the directory");
    return;
  }

  while (file.openNext(&dir, O_RDONLY)) {
    char fileName[255];
    size_t len = file.getName8(fileName, sizeof(fileName));


  // Check if fileName contains a space character
  if (strchr(fileName, ' ') != NULL) {
    // If it does, modify the variable to contain enclosing quotes
    memmove(&fileName[1], &fileName[0], len + 1); // Shift the string to make space for the leading quote
    fileName[0] = '"';
    fileName[len + 1] = '"';
    fileName[len + 2] = '\0';
  }

    if(file.isDirectory()){
      shell.setTerminalCharacterColor(response, 1, 34);
      response -> print(fileName);
      response -> print("/");
      shell.setTerminalCharacterColor(response, 0, 37);
      response -> print(" ");
    }else{
      response -> print(fileName);
      response -> print(" ");
    }
    file.close();
  }
  dir.close();
}




int countForwardSlashes(String str) {
  int count = 0;

  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == '/') {
      count++;
    }
  }
  return count;
}

String getParentFolder(String currentPathEdited) {
  String parentFolder = "";

  int countSlashes = countForwardSlashes(currentPathEdited);
  // we are going to the root
  if(countSlashes == 2){
    return "/";
  }

  // Find the index of the last occurrence of '/'
  int lastSlashIndex = currentPathEdited.lastIndexOf('/');

  // Check if the last folder is not the root folder
  if (lastSlashIndex > 0) {
    // Get the parent folder by extracting the substring up to the last '/'
    parentFolder = currentPathEdited.substring(0, lastSlashIndex);

    // Find the index of the previous-to-last occurrence of '/'
    lastSlashIndex = parentFolder.lastIndexOf('/');

    // Check if the parent folder is not the root folder
    if (lastSlashIndex > 0) {
      // Get the grandparent folder by extracting the substring up to the previous-to-last '/'
      parentFolder = parentFolder.substring(0, lastSlashIndex + 1);
    } else {
      // If the parent folder is the root folder, append a '/' character
      parentFolder += '/';
    }
  }

  // avoid cases of double slashes
  parentFolder.replace("//", "/");

  // all done
  return parentFolder;
}



// change current working directory
void func_cd(char *args, Stream *response) {
  if (args == NULL) {
    response->println("No directory specified");
    return;
  }

  String argString = String(args);

  // handle special cases
  if(argString.equals("..")){
    argString = getParentFolder(currentPath);
  }

  if(argString.equals("../")){
    String parentFolder = getParentFolder(currentPath);
    argString = argString.substring(2, argString.length());
    argString = parentFolder + argString;
  }



  // open directory
  File32 dir;
  if (!dir.open(argString.c_str(), O_RDONLY)) {
    response->println("Directory not found: " + argString);
    return;
  }

  // check if directory
  if (!dir.isDirectory()) {
    response->println("Not a directory: " + argString);
    dir.close();
    return;
  }

  // change directory
  if (!sd.chdir(argString.c_str())) {
    response->println("Failed to change directory: " + argString);
    dir.close();
    return;
  }

  // update the current path
  if (argString.startsWith("/")) { // check if string starts with "/"
    currentPath = argString;
  } else {
    // always add "/" at the end
    currentPath = currentPath  + argString + "/";
  }

  // avoid double slashes
  currentPath.replace("//", "/");

  response->println("Current path: " + currentPath);
  // close directory
  dir.close();
}

