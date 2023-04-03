
#ifndef TERMINAL_CPP
#define TERMINAL_CPP


#include <lvgl.h>
#include <WiFi.h>
#include "esp_wifi.h"

#include "core/blum_global.h"
#include "hardware/wifi/module_wifi.hpp"

//  Terminal that can be accessed from the wifi network

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include "terminal.hpp"
#include "commands_internal.hpp"
#include "commands_disk.hpp"

boolean hasWiFiShellStarted = false;

// Define serverWifi here in only one source file
WiFiServer serverWifi( SERVER_PORT );

// We have to create an object from Commander class.
Commander commander;

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell( 
  &serverWifi//, executionFunction
  );

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shellSerial( &Serial );

Commander::API_t API_tree[] = {
    // custom commands
    apiElement( "clear", "Clear the screen contents", func_clear),
    apiElement( "formatCard", "Format (erase/initialize) the memory card", func_formatCard),
    apiElement( "echo", "Print a line of text\r\n\tExample: echo [ message ]", func_echo),
    apiElement( "ls", "List files in a single line", func_ls),
    apiElement( "ll", "List files with details", func_ll),
    apiElement( "mkdir", "Make a directory\r\n\tExample: mkdir [ Folder Name ]", func_mkdir),
    apiElement( "cd", "Change directory\r\n\tExample: cd [ Folder Name ]", func_cd),
    apiElement( "rm", "Remove one file", func_rm),
    apiElement( "touch", "Create an empty file\r\n\tExample: touch [ File Name ]", func_touch),
    apiElement( "reboot", "Reboots the device", func_reboot),
    apiElement( "wait", "Wait a number of milliseconds\r\n\tExample: wait 5000", func_wait),
    apiElement( "whoami", "Displays the current user", func_wait),
    apiElement( "run", "Runs an app from disk\r\n\tExample: run [ File Name ]", func_run),
    apiElement( "version", "Outputs operating system version", func_version),
    apiElement( "logo", "Outputs operating system logo", func_logo),


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


void loopTerminal() {
  if(isWiFiConnected() == false){
    return;
  }

  if(hasWiFiShellStarted == false){
    setupTerminal();
  }

  shell.update();
}


void setupTerminal() {
  if(isWiFiConnected() == false){
    return;
  }

 // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Print start message
  Serial.println( "Program begin..." );


  shell.beginServer();
  Serial.println( "[OK]" );


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

  // we are ready to start
  hasWiFiShellStarted = true;
}


#endif
