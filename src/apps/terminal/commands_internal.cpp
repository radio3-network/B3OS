#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include "core/blum_global.h"
#include "terminal.hpp"


// prints a line of text
void func_whoami(char *args, Stream *response ){
  response -> print("root");
}


// delays execution for some seconds
void func_wait(char *args, Stream *response ){
  if (args == NULL) {
    response->print("No milliseconds specified");
    return;
  }
  uint32_t value = strtoul(args, NULL, 10);
  delay(value);
}


// similar to CLEAR function in Linux
void func_clearWiFi(char *args, Stream *response ){
  shellWiFi.clear();
  //Shellminator::clear(response);
}

// reboot the device
void func_reboot(char *args, Stream *response ){

    if( strcmp( args, "now" ) == 0 ){
      response -> println( "Please wait, the system will reboot..." );
      return;
    }

    response -> print("Reboot the system now?\r\n[y/n]: ");
    
    if( Shellminator::waitForKey( response, "yY", 30000 ) ){
      response -> println( "Please wait, the system will reboot..." );
      ESP.restart();
      //esp_restart();
    }

    else{
      response -> println( "Reboot aborted!" );
    }

}

// prints a line of text
void func_echo(char *args, Stream *response ){
  response -> print(args);
}

// prints the current version
void func_version(char *args, Stream *response ){
  response -> print(version);
}

// prints the logo of the operating system
void func_logoWiFi(char *args, Stream *response ){
  //response -> print(logo);
  shellWiFi.printBanner();
}

/*
// prints the logo of the operating system
void func_exit(char *args, Stream *response ){
  response -> print(0x03);
}
*/

// sends a beep to the console (not all support this)
void func_beepWiFi(char *args, Stream *response ){
    shellWiFi.beep();
}
