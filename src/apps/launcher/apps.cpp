#include <SdFat.h>
#include <esp_partition.h>

/**
 * Provides a safe offset for loading the binary from disk storage
 */
uint32_t calculate_flash_offset(File32 file) {
  const esp_partition_t* partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
  uint32_t max_binary_app_size = partition->size;
  uint32_t file_size = file.size();

  Serial.print("Maximum binary app size that can be loaded into partition: ");
  Serial.println(max_binary_app_size);

  Serial.print("Size of binary app to be loaded: ");
  Serial.println(file_size);

  // Check if file size is larger than available free memory
  if (file_size > max_binary_app_size) {
    Serial.println("File size is larger than available free memory");
    return -1;
  }

  /*
  Ensure that the binary app is placed in a location
  that is aligned with the memory requirements of the ESP32,
  while also providing enough space for it to run reliably.
  */
  uint32_t offset = partition->address + ((max_binary_app_size - file_size) / 2);
  return offset;
}

void writeFromFileToFlash(uint32_t address, File32 file) {
    // Open file
    if (!file) {
        Serial.print("Failed to open app file");
        return;
    }

    // Get file size
    size_t fileSize = file.size();

    // Find partition
    const esp_partition_t* partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
    if (!partition) {
        Serial.println("Failed to find partition for writing");
        file.close();
        return;
    }

    // Erase flash memory
    esp_err_t err = esp_partition_erase_range(partition, address, fileSize);
    if (err != ESP_OK) {
        Serial.println("Failed to erase flash memory");
        file.close();
        return;
    }

    // Allocate buffer for file data
    byte* buffer = new byte[fileSize];

    // Read file data into buffer
    if (file.read(buffer, fileSize) != fileSize) {
        Serial.println("Failed to read app file");
        delete[] buffer;
        file.close();
        return;
    }

    // Write data to flash memory
    err = esp_partition_write(partition, address, buffer, fileSize);
    if (err != ESP_OK) {
        Serial.println("Failed to write data to flash memory");
        delete[] buffer;
        file.close();
        return;
    }

    // Free buffer memory and close file
    delete[] buffer;
    file.close();

    Serial.println("Data written to flash memory successfully");
}


/**
 *  Runs a binary app.
 *  Returns false when something went wrong.
 */
boolean runFile(File32 file) {
    uint32_t address = calculate_flash_offset(file);
    if(address == -1){
        Serial.println("Unable to load app from file");
        return false;
    }
    writeFromFileToFlash(address, file);
    // now run the app
    void (*firmwareEntry)(void) = (void (*)())(address);
    firmwareEntry();
    return true;
}

