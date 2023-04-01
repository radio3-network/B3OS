#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include <WiFi.h>
#include "SdFat.h"
#include "sdios.h"

extern WiFiServer serverWifi;
extern SdFat sd;
extern Shellminator shell;

// Function declarations
void setupTerminal();
void loopTerminal();

#endif
