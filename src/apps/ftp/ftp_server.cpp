#include <Arduino.h>
#include "SdFat.h"
//#include <SimpleFTPServer.h>

#include "apps/terminal/commands_disk.hpp"

//#include <WiFi.h>
//#include <WiFiClient.h>
#include "ESP32FtpServer.h"

const char* ssid = "blablabla..."; //WiFi SSID
const char* password = "blablabla..."; //WiFi Password

//FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial


//FtpServer ftpSrv;

void setupFTP()
{
  //  ftpSrv.begin("root","root");    //username, password for ftp.
}


void loopFTP() {
  	//  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!
  }