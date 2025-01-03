// Includes
#include "FloppyFiles.hpp"
#include "FloppyHardware.hpp"
#include <SPIFFS.h>
#include <FS.h>

// Constants
#define FORMAT_SPIFFS_IF_FAILED true

// Constructor
FloppyFiles::FloppyFiles()
{
}

// Functions
uint8_t FloppyFiles::initFS()
{
  return SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
}

uint8_t FloppyFiles::initSettings()
{
  return this->preferences.begin("settings");
}

uint32_t FloppyFiles::getFreeSpace(void)
{
  return SPIFFS.totalBytes() - SPIFFS.usedBytes();
}

uint8_t FloppyFiles::readFileToSerial(const char *filename)
{
  // Open the file
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
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
  while (file.available())
  {
    byte data = file.read(); // Read one byte from the file
    Serial.write(data);      // Send this byte to the serial port
    crc ^= data;             // Update the CRC32 calculation

    // Perform the CRC32 computation
    for (int i = 0; i < 8; i++)
    {
      if (crc & 1)
      {
        crc = (crc >> 1) ^ 0xEDB88320; // Standard CRC32 polynomial
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  // Send the end of file marker
  FloppyHardware::print("\x04"); // End of transmission
  FloppyHardware::print("\n");   // End of file

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

uint8_t FloppyFiles::writeFileFromSerial(const char *filename)
{
  // Open the file
  File file = SPIFFS.open(filename, "w");
  if (!file)
  {
    FloppyHardware::print("Error: File not found\n");
    return 0;
  }

  // Initialize CRC32
  unsigned long crc = 0xFFFFFFFF;

  // Read the file, send to serial, and compute CRC32 simultaneously
  while (true)
  {
    // Read one byte from the serial port
    while (!FloppyHardware::available())
    {
      delay(1);
    }
    char data = FloppyHardware::read();

    // Check for the end of file marker
    if (data == 0x04)
    {
      break;
    }

    // Write the byte to the file
    file.write(data);
    crc ^= data; // Update the CRC32 calculation

    // Perform the CRC32 computation
    for (int i = 0; i < 8; i++)
    {
      if (crc & 1)
      {
        crc = (crc >> 1) ^ 0xEDB88320; // Standard CRC32 polynomial
      }
      else
      {
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

void FloppyFiles::setString(const char *key, const char *value)
{
  this->preferences.putString(key, value);
}

const char *FloppyFiles::getString(const char *key)
{
  String value = this->preferences.getString(key);

  // Allocate memory for the result
  char *result = (char *)malloc(value.length() + 1); // +1 for the null terminator
  if (result != nullptr)
  {
    strcpy(result, value.c_str()); // Copy the string to the allocated memory
  }
  return result;
}

void FloppyFiles::setInt(const char *key, uint32_t value)
{
  this->preferences.putInt(key, value);
}

uint32_t FloppyFiles::getInt(const char *key)
{
  return this->preferences.getInt(key);
}

void FloppyFiles::setChar(const char *key, char value)
{
  this->preferences.putChar(key, value);
}

char FloppyFiles::getChar(const char *key)
{
  return this->preferences.getChar(key);
}