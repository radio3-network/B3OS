#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include <WiFi.h>


extern WiFiServer serverWifi;
extern Shellminator shell;
extern Shellminator shellSerial;

// Function declarations
void setupTerminal();
void loopTerminal();

#endif
