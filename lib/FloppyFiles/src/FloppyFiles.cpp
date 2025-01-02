// Includes
#include "FloppyFiles.hpp"
#include "FloppyHardware.hpp"
#include <SPIFFS.h>
#include <FS.h>

// Functions
uint32_t FloppyFiles::getFreeSpace(void)
{
  return SPIFFS.totalBytes() - SPIFFS.usedBytes();
}

uint8_t FloppyFiles::readFileToSerial(const char* filename)
{
  // Open the file
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    FloppyHardware::print("Error: File not found\n");
    return 0;
  }

  // Metadata
  FloppyHardware::print(filename);
  FloppyHardware::print("\n");

  // File size
  char size[10];
  itoa(file.size(), size, 10);
  FloppyHardware::print(size);
  FloppyHardware::print("\n");

  // Initialize CRC32
  unsigned long crc = 0xFFFFFFFF;

  // Read the file, send to serial, and compute CRC32 simultaneously
  while (file.available()) {
    byte data = file.read();      // Read one byte from the file
    Serial.write(data);           // Send this byte to the serial port
    crc ^= data;                  // Update the CRC32 calculation

    // Perform the CRC32 computation
    for (int i = 0; i < 8; i++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320; // Standard CRC32 polynomial
      } else {
        crc >>= 1;
      }
    }
  }

  
  // Send the end of file marker
  FloppyHardware::print("\x04"); // End of transmission
  FloppyHardware::print("\n"); // End of file

  file.close();

  // Finalize CRC32
  crc = ~crc;

  // Print the computed CRC32
  char crcStr[10];
  sprintf(crcStr, "%08X", crc); // Convert to hexadecimal string (8 digits)
  FloppyHardware::print(crcStr);
  FloppyHardware::print("\n");

  return 1;
}

uint8_t FloppyFiles::writeFileFromSerial(const char* filename)
{
  // Open the file
  File file = SPIFFS.open(filename, "w");
  if (!file) {
    FloppyHardware::print("Error: File not found\n");
    return 0;
  }

  // Command and metadata
  

  // Initialize CRC32
  unsigned long crc = 0xFFFFFFFF;

  // Read the file, send to serial, and compute CRC32 simultaneously
  while (true) {
    // Read one byte from the serial port
    while (!FloppyHardware::available()) {
      delay(1);
    }
    char data = FloppyHardware::read();

    // Check for the end of file marker
    if (data == 0x04) {
      break;
    }

    // Write the byte to the file
    file.write(data);
    crc ^= data;                  // Update the CRC32 calculation

    // Perform the CRC32 computation
    for (int i = 0; i < 8; i++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320; // Standard CRC32 polynomial
      } else {
        crc >>= 1;
      }
    }
  }

  file.close();

  // Finalize CRC32
  crc = ~crc;

  // Print the computed CRC32
  char crcStr[10];
  sprintf(crcStr, "%08X", crc); // Convert to hexadecimal string (8 digits)
  FloppyHardware::print(crcStr);
  FloppyHardware::print("\n");

  return 1;
}