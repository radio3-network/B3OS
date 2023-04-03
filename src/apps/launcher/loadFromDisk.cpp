#include <SPIFFS.h>
#include <FS.h>
#include <SD.h>
#include <SdFat.h>
#include <LittleFS.h>


bool loadAndExecuteFirmwareFromSDCard1(const char* filename, uint32_t flashOffset) {
    // initialize SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("Failed to initialize SPIFFS");
        return false;
    }

    // initialize the SD card
    if (!SD.begin()) {
        Serial.println("Failed to initialize SD card");
        return false;
    }

    // open the firmware file on the SD card
    File appFile = SD.open(filename, FILE_READ);
    if (!appFile) {
        Serial.println("Failed to open firmware file");
        return false;
    }

    // check the firmware file size
    size_t appSize = appFile.size();
    if (appSize == 0) {
        Serial.println("Firmware file is empty");
        return false;
    }

    // seek to the beginning of the firmware file
    appFile.seek(0);

    // allocate a buffer to hold the firmware binary image
    uint8_t* firmwareBuffer = new uint8_t[appSize];

    // read the firmware binary image into the buffer
    size_t bytesRead = appFile.read(firmwareBuffer, appSize);
    if (bytesRead != appSize) {
        Serial.println("Failed to read firmware file");
        delete[] firmwareBuffer;
        return false;
    }

    // open a new file for writing to the flash memory
    File firmwareFlashFile = SPIFFS.open("/firmware.bin", FILE_WRITE);
    if (!firmwareFlashFile) {
        Serial.println("Failed to open file for writing to flash memory");
        delete[] firmwareBuffer;
        return false;
    }

    //firmwareFlashFile.;

    // write the firmware binary image to the flash memory
    size_t bytesWritten = firmwareFlashFile.write(firmwareBuffer, appSize);
    firmwareFlashFile.close();
    if (bytesWritten != appSize) {
        Serial.println("Failed to write firmware to flash memory");
        delete[] firmwareBuffer;
        return false;
    }

    // execute the firmware code
    void (*firmwareEntry)(void) = (void (*)())(flashOffset);
    firmwareEntry();

    // free the buffer
    delete[] firmwareBuffer;

    // return success
    return true;
}



/**
 * Provides a safe offset for loading the binary from disk storage
 */
uint32_t calculate_flash_offset(File32 file) {
    uint32_t sketch_size = ESP.getSketchSize();
    uint32_t max_binary_app_size = ESP.getFlashChipSize() - sketch_size;
    uint32_t file_size = file.size();
    /*
    ensure that the binary app is placed in a location
    that is aligned with the memory requirements of the ESP32,
    while also providing enough space for it to run reliably.
    */
    uint32_t offset = sketch_size + ((max_binary_app_size - file_size) / 2);
    return offset;
}
