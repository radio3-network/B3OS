#include <Arduino.h>
#include "SdFat.h"
#include "apps/terminal/commands_disk.hpp"
#include <WiFi.h>
#include <WiFiClient.h>
#include "core/blum_global.h"

#include "ESP32FtpServer.h"

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial


void setupFTP()
{
    Serial.println("Setup FTP");
    // there is no wifi to start
    if (wifiEnabled == false) {
        Serial.println("FTP not enabled because WiFi isn't available");
        return;
    }
    
    ftpSrv.begin("root","root");    //username, password for ftp.
    Serial.println("FTP is running");
}


void loopFTP() {
    if (wifiEnabled == false) {
        return;
    }
  	  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!
  }