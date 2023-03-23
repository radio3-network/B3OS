
#ifndef BLUM_TERMINAL
#define BLUM_TERMINAL


/*
    Terminal that can be accessed from the wifi network
*/
#include <Shellminator.hpp>

#include <WiFi.h>
#include "esp_wifi.h"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "blum_terminal.hpp"

#define SERVER_PORT 23

// WiFi credentials.
static const char* ssid     = "---___---";
static const char* password = "vodafone";

// Create an instance of the server.
// It will be available on port 23.


// Define serverWifi here in only one source file
WiFiServer serverWifi( SERVER_PORT );


// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell( &serverWifi );

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"

;


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

  // initialize shell object.
  shell.begin( "arnold" );

}


void loopTerminal() {

  shell.update();

}

#endif
