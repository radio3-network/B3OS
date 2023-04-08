#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"
#include "Commander-API-Commands.hpp"

#include <WiFi.h>
#include <HTTPClient.h>

#include <hardware/hardware.h>
#include "commands_disk.hpp"
#include "terminal.hpp"

#include "core/blum_global.h"

// Storage card
#include "SdFat.h"
#include "sdios.h"

uint32_t cardSectorCount = 0;
uint8_t sectorBuffer[512];
// SdCardFactory constructs and initializes the appropriate card.
SdCardFactory cardFactory;
// Pointer to generic SD card.
SdCard *m_card = nullptr;

#define SPI_CLOCK SD_SCK_MHZ(6)
#define SD_CONFIG SdSpiConfig(TF_PIN_CS, DEDICATED_SPI, SPI_CLOCK)

String currentPath = "/";
SdFat sd;

void card_initialize() {
    // Select and initialize proper card driver.
    m_card = cardFactory.newCard(SD_CONFIG);
    if (!m_card || m_card->errorCode()) {
        Serial.println("Error initializing card factory");
        return;
    }

    cardSectorCount = m_card->sectorCount();
    if (!cardSectorCount) {
        Serial.println("Get sector count failed.");
        return;
    }

    if (!sd.begin(SD_CONFIG)) {
        Serial.println("Error initializing storage card.");
    }
}

// format a TF/SD card available on a single slot
void func_formatCard(char *args, Stream *response) {
    ExFatFormatter exFatFormatter;
    FatFormatter fatFormatter;

    // Format exFAT if larger than 32GB.
    bool rtn = cardSectorCount > 67108864 ? exFatFormatter.format(m_card, sectorBuffer, &Serial) : fatFormatter.format(m_card, sectorBuffer, &Serial);

    if (!rtn) {
        response->println("Error formatting storage card.");
        return;
    }

    response->println("Storage card formatted successfully.");
    // sd.end();
}

/**
 * Returns the canonical path for a char with a file name
 */
String getPath(char *args) {
    // convert args to String
    String argString = String(args);
    // canonical path
    if (argString.startsWith("/")) {
        return argString;
    } else {
        return currentPath + argString;
    }
}

void func_touch(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file name specified");
        return;
    }

    String path = getPath(args);
    // open file in write mode
    File32 file = sd.open(path, FILE_WRITE);
    if (!file) {
        response->println("Failed to create file: " + path);
        return;
    }

    file.close();
    response->println("File created");
}

void func_mkdir(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file or folder specified");
        return;
    }
    String path = getPath(args);
    if (!sd.mkdir(path)) {
        response->println("Failed to create folder");
    }
}

void func_rm(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file or folder specified");
        return;
    }
    String path = getPath(args);
    if (!sd.remove(path)) {
        response->println("Failed to remove file");
    }
}

// Helper function to zero-pad a number to two digits
String zeroPad(int n) {
    if (n < 10) {
        return "0" + String(n);
    } else {
        return String(n);
    }
}

String getModifyDateTime(FatFile file) {
    uint16_t date, time;
    if (!file.getModifyDateTime(&date, &time)) {
        return "";  // Return empty string on error
    }

    // Extract year, month, and day from the date
    uint16_t year = ((date >> 9) & 0x7F) + 1980;
    uint8_t month = (date >> 5) & 0x0F;
    uint8_t day = date & 0x1F;

    // Extract hour, minute, and second from the time
    uint8_t hour = (time >> 11) & 0x1F;
    uint8_t minute = (time >> 5) & 0x3F;
    uint8_t second = (time & 0x1F) * 2;

    // Format the date and time as a string
    return String(year) + "-" + zeroPad(month) + "-" + zeroPad(day) + " " + zeroPad(hour) + ":" + zeroPad(minute) + ":" + zeroPad(second);
}

// prints a line of text
void func_ll(char *args, Stream *response) {
    File32 dir;
    File32 file;

    // open the current directory
    if (!dir.openCwd()) {
        response->println("Failed to open the directory");
        return;
    }

    while (file.openNext(&dir, O_RDONLY)) {
        char fileName[255];
        size_t len = file.getName8(fileName, sizeof(fileName));

        String time = getModifyDateTime(file);
        response->print(time + " ");

        if (file.isDirectory()) {
            // 34 == blue, 37 = white, 32 = green
            Shellminator::setTerminalCharacterColor(response, Shellminator::BOLD, Shellminator::BLUE);
            response->print(fileName);
            response->print("/");
            Shellminator::setTerminalCharacterColor(response, Shellminator::REGULAR, Shellminator::WHITE);
            response->print("\r\n");
        } else {
            response->print(fileName);
            response->print("\r\n");
        }
        file.close();
    }
    dir.close();
}

// prints a line of text
void func_ls(char *args, Stream *response) {
    File32 dir;
    File32 file;

    // open the current directory
    if (!dir.openCwd()) {
        response->println("Failed to open the directory");
        return;
    }

    while (file.openNext(&dir, O_RDONLY)) {
        char fileName[255];
        size_t len = file.getName8(fileName, sizeof(fileName));

        // Check if fileName contains a space character
        if (strchr(fileName, ' ') != NULL) {
            // If it does, modify the variable to contain enclosing quotes
            memmove(&fileName[1], &fileName[0], len + 1);  // Shift the string to make space for the leading quote
            fileName[0] = '"';
            fileName[len + 1] = '"';
            fileName[len + 2] = '\0';
        }

        if (file.isDirectory()) {
            Shellminator::setTerminalCharacterColor(response, Shellminator::BOLD, Shellminator::BLUE);
            response->print(fileName);
            response->print("/");
            Shellminator::setTerminalCharacterColor(response, Shellminator::REGULAR, Shellminator::WHITE);
            response->print(" ");
        } else {
            response->print(fileName);
            response->print(" ");
        }
        file.close();
    }
    dir.close();
}

int countForwardSlashes(String str) {
    int count = 0;

    for (int i = 0; i < str.length(); i++) {
        if (str.charAt(i) == '/') {
            count++;
        }
    }
    return count;
}

String getParentFolder(String currentPathEdited) {
    String parentFolder = "";

    int countSlashes = countForwardSlashes(currentPathEdited);
    // we are going to the root
    if (countSlashes == 2) {
        return "/";
    }

    // Find the index of the last occurrence of '/'
    int lastSlashIndex = currentPathEdited.lastIndexOf('/');

    // Check if the last folder is not the root folder
    if (lastSlashIndex > 0) {
        // Get the parent folder by extracting the substring up to the last '/'
        parentFolder = currentPathEdited.substring(0, lastSlashIndex);

        // Find the index of the previous-to-last occurrence of '/'
        lastSlashIndex = parentFolder.lastIndexOf('/');

        // Check if the parent folder is not the root folder
        if (lastSlashIndex > 0) {
            // Get the grandparent folder by extracting the substring up to the previous-to-last '/'
            parentFolder = parentFolder.substring(0, lastSlashIndex + 1);
        } else {
            // If the parent folder is the root folder, append a '/' character
            parentFolder += '/';
        }
    }

    // avoid cases of double slashes
    parentFolder.replace("//", "/");

    // all done
    return parentFolder;
}

// change current working directory
void func_cd(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->print("No directory specified");
        return;
    }

    String argString = String(args);

    // handle special cases
    if (argString.equals("..")) {
        argString = getParentFolder(currentPath);
    }

    if (argString.equals("../")) {
        String parentFolder = getParentFolder(currentPath);
        argString = argString.substring(2, argString.length());
        argString = parentFolder + argString;
    }

    // open directory
    File32 dir;
    if (!dir.open(argString.c_str(), O_RDONLY)) {
        response->println("Directory not found: " + argString);
        return;
    }

    // check if directory
    if (!dir.isDirectory()) {
        response->println("Not a directory: " + argString);
        dir.close();
        return;
    }

    // change directory
    if (!sd.chdir(argString.c_str())) {
        response->print("Failed to change directory: " + argString);
        dir.close();
        return;
    }

    // update the current path
    if (argString.startsWith("/")) {  // check if string starts with "/"
        currentPath = argString;
    } else {
        // always add "/" at the end
        currentPath = currentPath + argString + "/";
    }

    // avoid double slashes
    currentPath.replace("//", "/");

    shellSerial.setBannerPathText(currentPath.c_str());
    shell.setBannerPathText(currentPath.c_str());
    

    response->print("Current path: " + currentPath);
    // close directory
    dir.close();
}

void func_run(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->print("No file name specified");
        return;
    }

    String path = getPath(args);
    // open file in read mode
    File32 file = sd.open(path, FILE_READ);
    if (!file) {
        response->print("Failed to open file: " + path);
        return;
    }

    response->println("Running " + path);
    runFile(file, response);

    file.close();
    response->print("App concluded");
}

void downloadFile(String url, String filename, Stream *response) {
    WiFiClient client;

    // Create an HTTP client object
    HTTPClient http;

    // Start the download
    response->printf("Downloading %s", url.c_str());
    response->println("");
    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        // Open the file on the SD card for writing
        response->printf("Saving to %s", filename.c_str());
        response->println("");

        File32 file = sd.open(filename, FILE_WRITE);

        // Write the downloaded data to the file
        WiFiClient *stream = http.getStreamPtr();
        int totalSize = http.getSize();
        int downloadedSize = 0;
        while (stream->available()) {
            byte data = stream->read();
            file.write(data);
            downloadedSize++;
            if (downloadedSize % 1024 == 0) {
                int progress = (int)(downloadedSize * 100 / totalSize);
                response->printf("%d/%d bytes (%d%%)", downloadedSize, totalSize, progress);
                response->println("");
            }
        }

        // Close the file
        file.close();
        response->println("Download complete");
    } else {
        response->printf("Failed to download %s (HTTP error %d)", url.c_str(), httpCode);
        response->println("");
    }

    // Cleanup
    http.end();
}

void func_download(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file or folder specified");
        return;
    }

    // Parse the arguments string
    char *url = strtok(args, " ");
    char *filename_c = strtok(NULL, " ");

    if (url == NULL || filename_c == NULL) {
        // Invalid arguments
        response->println("Invalid arguments.");
        return;
    }

    // get the proper path
    String filename = getPath(filename_c);

    // Download the file
    downloadFile(url, filename, response);
}

void func_print(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file name specified");
        return;
    }

    String filename = getPath(args);

    // Open the file
    File32 file = sd.open(filename);

    // Check if the file opened successfully
    if (!file) {
        response->println("Failed to open file");
        return;
    }

    // Print the file contents to the response stream
    while (file.available()) {
        response->write(file.read());
    }

    // Close the file
    file.close();
}
