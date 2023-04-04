#include <Arduino.h>
#include <string.h>
#include "SdFat.h"

#include "commands_disk.hpp"

#include "core/blum_global.h"


//#include "ESP32-FTP-Server.h"
//#include "FTPFilesystem.h"

#define FTP_USER "ftp"
#define FTP_PASSWORD "ftp"

//FTPServer ftp;


void func_ftpserver(char *args, Stream *response) {
    /*
    if (strcmp(args, "stop") == 0) {
        // stop FTP server
        response->println("FTP server stopped");
        //ftp.stop();
        //SPIFFS.end();
        ftpServerRunning = false;
        return;
    }
    // start FTP server
    //SPIFFS.begin(true);
    ftp.addUser(FTP_USER, FTP_PASSWORD);
    //ftp.addFilesystem("SPIFFS", &SPIFFS);
    ftp.addFilesystem(&sdfs);
    ftp.begin();
    ftpServerRunning = true;
    response->println("FTP server started on port 21");
    */
}

void ftpHandle(){
//    ftp.handle();
}

