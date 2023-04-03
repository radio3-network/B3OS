#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include "terminal.hpp"


// prints a line of text
void func_whoami(char *args, Stream *response ){
  response -> println("root");
}


// delays execution for some seconds
void func_wait(char *args, Stream *response ){
  if (args == NULL) {
    response->println("No milliseconds specified");
    return;
  }
  uint32_t value = strtoul(args, NULL, 10);
  delay(value);
}


// similar to CLEAR function in Linux
void func_clear(char *args, Stream *response ){
  shell.clear();
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
      esp_restart();
    }

    else{
      response -> println( "Reboot aborted!" );
    }

}

// prints a line of text
void func_echo(char *args, Stream *response ){
  response -> print(args);
  response -> print("\r\n");
}
