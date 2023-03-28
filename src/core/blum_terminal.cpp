
#ifndef BLUM_TERMINAL
#define BLUM_TERMINAL


#include <lvgl.h>
#include <WiFi.h>
#include "esp_wifi.h"

#include "SdFat.h"
#include "sdios.h"

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
uint32_t cardSectorCount = 0;
uint8_t  sectorBuffer[512];
// SdCardFactory constructs and initializes the appropriate card.
SdCardFactory cardFactory;
// Pointer to generic SD card.
SdCard* m_card = nullptr;

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

Commander::API_t API_tree[] = {
    apiElement( "clear", "Clear the screen contents", func_clear),
    apiElement( "formatCard", "Format (erase/initialize) the memory card", func_formatCard),
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
  //response -> print("Clearing the screen!\r\n");
}

// similar to CLEAR function in Linux
void func_formatCard(char *args, Stream *response ){


  // Select and initialize proper card driver.
  m_card = cardFactory.newCard(SD_CONFIG);
  if (!m_card || m_card->errorCode()) {
    response -> println("Error initializing SD card.");
    return;
  }

  cardSectorCount = m_card->sectorCount();
  if (!cardSectorCount) {
    response -> println("Get sector count failed.");
    return;
  }

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
/*
    if (!SD.begin(TF_PIN_CS)) {
      response -> println("Error initializing SD card.");
      return;
    }

    response -> println("Formatting storage card...");
    
   /* if (SD.format()) {
      response -> println("Storage card formatted successfully.");
    } else {
      response -> println("Error formatting storage card.");
    }
*/
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



  //microbox.begin(&Params[0], hostname, true, historyBuf, 100);

  // add the handler to run inside LVGL
  //
  //lv_timer_t * timer = lv_timer_create(loopTerminal2, 10,  NULL);
  //lv_timer_ready(timer);
  
}

/*
void executionFunction( char* command ){

  if(strcmp(command,"clear")==0){
    shell.clear();
    return;
  }
  
  if(strcmp(command,"pin")==0){
    shell.setTerminalCharacterColor(Shellminator::BOLD, Shellminator::GREEN);
    Serial.println("Writing to pin");
    shell.setTerminalCharacterColor(Shellminator::REGULAR, Shellminator::WHITE);
    return;
  }

  Serial.print( "'");
  shell.setTerminalCharacterColor(Shellminator::BOLD, Shellminator::RED);
  Serial.println( command );
  Serial.print( "'");
  shell.setTerminalCharacterColor(Shellminator::REGULAR, Shellminator::WHITE);
  Serial.println(" is not a command");
}
*/

#endif
