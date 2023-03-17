#include <Arduino.h>
#include <EEPROM.h>

#define MAX_KEY_SIZE 32
#define MAX_VALUE_SIZE 32

static int mNextAddress;

class TextKeyValueStore {
  public:
    static void begin(size_t size) {
      EEPROM.begin(size);
      mNextAddress = 0;
    }

    static void put(const char* key, const char* value) {
      size_t keySize = strlen(key);
      size_t valueSize = strlen(value);

      EEPROM.put(mNextAddress, keySize);
      mNextAddress += sizeof(keySize);

      EEPROM.put(mNextAddress, valueSize);
      mNextAddress += sizeof(valueSize);

      for (int i = 0; i < keySize; i++) {
        EEPROM.write(mNextAddress++, key[i]);
      }

      for (int i = 0; i < valueSize; i++) {
        EEPROM.write(mNextAddress++, value[i]);
      }

      EEPROM.commit();
    }

    static const char* get(const char* key) {
      int address = 0;
      int keySize = 0;
      int valueSize = 0;

      while (address < EEPROM.length()) {
        EEPROM.get(address, keySize);
        address += sizeof(keySize);

        EEPROM.get(address, valueSize);
        address += sizeof(valueSize);

        if (address + keySize + valueSize > EEPROM.length()) {
          break;
        }

        char keyBuffer[MAX_KEY_SIZE];
        for (int i = 0; i < keySize; i++) {
          keyBuffer[i] = EEPROM.read(address + i);
        }
        keyBuffer[keySize] = '\0';

        if (strcmp(key, keyBuffer) == 0) {
          char valueBuffer[MAX_VALUE_SIZE];
          for (int i = 0; i < valueSize; i++) {
            valueBuffer[i] = EEPROM.read(address + keySize + i);
          }
          valueBuffer[valueSize] = '\0';
          return valueBuffer;
        }

        address += keySize + valueSize;
      }

      return NULL;
    }
};
