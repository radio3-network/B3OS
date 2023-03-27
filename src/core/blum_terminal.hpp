#ifndef BLUM_TERMINAL_HPP
#define BLUM_TERMINAL_HPP

#include <WiFi.h>
#include <microBox.h>

extern WiFiServer serverWifi;

// Function declarations
void setupTerminal();
void loopTerminal();

#endif
