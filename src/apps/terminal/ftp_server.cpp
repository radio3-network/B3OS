#include <ESPAsyncWebServer.h>
#include <ESPAsyncFTP.h>

#include <Arduino.h> // required for Arduino functions and types
#include <string.h> // required for string functions
#include "SdFat.h"

#include "commands_disk.hpp"

AsyncWebServer ftpServer(21); // create server on port 21


void handleFileListRequest(AsyncFTPRequest *request, AsyncFTPResponse *response) {
    // get file list from SD card
    SdFile dir;
    dir.open("/", O_RDONLY);
    while (true) {
        SdFile file = dir.openNextFile();
        if (!file) break;
        if (file.isDirectory()) {
            response->addFile(String(file.name()) + "/", file.size(), false);
        } else {
            response->addFile(String(file.name()), file.size(), true);
        }
        file.close();
    }
    dir.close();
}

void func_ftpserver(char *args, Stream *response) {
    if (strcmp(args, "stop") == 0) {
        // stop FTP server
        server.end();
        response->println("FTP server stopped");
    } else {
        // start FTP server
        server.serveStatic("/", SD, "/", "max-age=86400");
        server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->redirect("/"); // redirect to root for file listing
        });
        server.on("/list", HTTP_FTP, handleFileListRequest); // handle file listing requests
        server.begin();
        response->println("FTP server started");
    }
}