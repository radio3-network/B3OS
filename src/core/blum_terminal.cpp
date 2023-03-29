
#ifndef BLUM_TERMINAL
#define BLUM_TERMINAL


#include <lvgl.h>
#include <WiFi.h>
#include "esp_wifi.h"

#include <hardware/hardware.h>


/*
    Terminal that can be accessed from the wifi network
*/
#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include "blum_terminal.hpp"

// Storage card
#include "SdFat.h"
#include "sdios.h"
uint32_t cardSectorCount = 0;
uint8_t  sectorBuffer[512];
// SdCardFactory constructs and initializes the appropriate card.
SdCardFactory cardFactory;
// Pointer to generic SD card.
SdCard* m_card = nullptr;

SdFat sd;
String currentPath = "/";

#define SPI_CLOCK SD_SCK_MHZ(6)
#define SD_CONFIG SdSpiConfig(TF_PIN_CS, DEDICATED_SPI, SPI_CLOCK)



// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command );

// WiFi credentials.
static const char* ssid     = "---___---";
static const char* password = "vodafone";

// Create an instance of the server.
// It will be available on port 23.
#define SERVER_PORT 23


// Define serverWifi here in only one source file
WiFiServer serverWifi( SERVER_PORT );

// We have to create an object from Commander class.
Commander commander;

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell( &serverWifi//, executionFunction
);

const char logo[] =

"                 _  \r\n" 
" ._ _.  _| o  _  _) \r\n" 
" | (_| (_| | (_) _) \r\n" 

"\r\n\033[0;37m"
"\033[1;32m https://radio3.network\r\n\r\n"

;

void func_clear( char *args, Stream *response );
void func_formatCard( char *args, Stream *response );
void func_echo( char *args, Stream *response );
void func_ls( char *args, Stream *response );
void func_mkdir( char *args, Stream *response );
void func_cd( char *args, Stream *response );
void func_rm( char *args, Stream *response );
void func_touch( char *args, Stream *response );
void func_reboot( char *args, Stream *response );

Commander::API_t API_tree[] = {
    // custom commands
    apiElement( "clear", "Clear the screen contents", func_clear),
    apiElement( "formatCard", "Format (erase/initialize) the memory card", func_formatCard),
    apiElement( "echo", "Print a line of text", func_echo),
    apiElement( "ls", "List files", func_ls),
    apiElement( "mkdir", "Make a directory", func_mkdir),
    apiElement( "cd", "Change directory", func_cd),
    apiElement( "rm", "Remove one file", func_rm),
    apiElement( "touch", "Create an empty file", func_touch),
    apiElement( "reboot", "Reboots the device", func_reboot),

    // built-in commands
    API_ELEMENT_MILLIS,
    API_ELEMENT_MICROS,
    API_ELEMENT_UPTIME,
    API_ELEMENT_PINMODE,
    API_ELEMENT_DIGITALWRITE,
    API_ELEMENT_DIGITALREAD,
    API_ELEMENT_ANALOGREAD,
    API_ELEMENT_IPCONFIG,
    API_ELEMENT_WIFISTAT,
    API_ELEMENT_WIFISCAN,
    API_ELEMENT_CONFIGTIME,
    API_ELEMENT_DATETIME,
    API_ELEMENT_NEOFETCH,
    API_ELEMENT_SIN,
    API_ELEMENT_COS,
    API_ELEMENT_ABS,
    API_ELEMENT_RANDOM,
    API_ELEMENT_NOT
};

// similar to CLEAR function in Linux
void func_clear(char *args, Stream *response ){
  shell.clear();
}

// reboot the device
void func_reboot(char *args, Stream *response ){

    response -> println("Reboot now? (Y/n)");
    while( !response -> available() ){
         // Waiting for user input
    }

    char c = response -> read();

    if( c == 'y' || c == 'Y' ){
        response -> println("System is rebooting NOW");
        esp_restart();
    }
}

// prints a line of text
void func_echo(char *args, Stream *response ){
  response -> print(args);
  response -> print("\r\n");
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
  File file = sd.open(path, FILE_WRITE);
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


// prints a line of text
void func_ls(char *args, Stream *response ){
  File dir;
  File file;

  // open the current directory
  if (!dir.openCwd()){
    response -> println("Failed to open the directory");
    return;
  }

  while (file.openNext(&dir, O_RDONLY)) {
    char fileName[13];
    size_t len = file.getName(fileName, sizeof(fileName));

    if(file.isDirectory()){
      response -> print(fileName);
      response -> print("/");
      response -> print("\r\n");
    }else{
      response -> print(fileName);
      response -> print("\r\n");
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

  return parentFolder;
}



// change current working directory
void func_cd(char *args, Stream *response) {
  if (args == NULL) {
    response->println("No directory specified");
    return;
  }

  String argString = String(args);
  if(argString.equals("..")){
    argString = getParentFolder(currentPath);
  }

  // open directory
  File dir;
  if (!dir.open(argString.c_str())) {
    response->println("Directory not found");
    return;
  }

  // check if directory
  if (!dir.isDirectory()) {
    response->println("Not a directory");
    dir.close();
    return;
  }

  // change directory
  if (!sd.chdir(argString.c_str())) {
    response->println("Failed to change directory");
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

  response->println("Current path: " + currentPath);
  // close directory
  dir.close();
}


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

// similar to CLEAR function in Linux
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





void loopTerminal() {
  shell.update();
}


void setupTerminal() {
 // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Print start message
  Serial.println( "Program begin..." );

  // WiFi configuration section
  Serial.print( "Connect to  WiFi: " );
  Serial.print( ssid );

  WiFi.mode( WIFI_STA );
  WiFi.setSleep(WIFI_PS_NONE);
  WiFi.begin( ssid, password );

  while( WiFi.status() != WL_CONNECTED ){

    delay( 1000 );
    Serial.print( "." );

  }

  shell.beginServer();

  Serial.println( " [ OK ]" );

  Serial.println( "Connected!" );
  Serial.print( "Device IP: " );
  Serial.print( WiFi.localIP() );
  Serial.print( " at port: " );
  Serial.println( SERVER_PORT );


  // Initialize the storage card
  card_initialize();

  // There is an option to attach a debug channel to Commander.
  // It can be handy to find any problems during the initialization
  // phase. In this example we will use Serial for this.
  commander.attachDebugChannel( &Serial );

  // At start, Commander does not know anything about our commands.
  // We have to attach the API_tree array from the previous steps
  // to Commander to work properly.
  commander.attachTree( API_tree );

  // Initialize Commander.
  commander.init();

  shell.attachCommander( &commander );

  // initialize shell object.
  shell.begin( "root" );

  // setup the current path
  currentPath = "/";
}


#endif
