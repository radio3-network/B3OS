
#ifndef TERMINAL_CPP
#define TERMINAL_CPP


#include <lvgl.h>
#include <WiFi.h>
#include "esp_wifi.h"


/*
  Terminal that can be accessed from the wifi network
*/
#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include "terminal.hpp"
#include "commands_internal.hpp"
#include "commands_disk.hpp"


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
Shellminator shell( 
  &serverWifi//, executionFunction
  );

const char logo[] =

"                 _  \r\n" 
" ._ _.  _| o  _  _) \r\n" 
" | (_| (_| | (_) _) \r\n" 

"\r\n\033[0;37m"
"\033[1;32m https://radio3.network\r\n\r\n";



Commander::API_t API_tree[] = {
    // custom commands
    apiElement( "clear", "Clear the screen contents", func_clear),
    apiElement( "formatCard", "Format (erase/initialize) the memory card", func_formatCard),
    apiElement( "echo", "Print a line of text", func_echo),
    apiElement( "ls", "List files in a single line", func_ls),
    apiElement( "ll", "List files with details", func_ll),
    apiElement( "mkdir", "Make a directory", func_mkdir),
    apiElement( "cd", "Change directory", func_cd),
    apiElement( "rm", "Remove one file", func_rm),
    apiElement( "touch", "Create an empty file", func_touch),
    apiElement( "reboot", "Reboots the device", func_reboot),
    apiElement( "wait", "Wait a number of milliseconds", func_wait),
    apiElement( "whoami", "Displays the current user", func_wait),

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
