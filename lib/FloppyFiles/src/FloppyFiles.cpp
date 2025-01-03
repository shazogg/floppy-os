// Includes
#include "FloppyFiles.hpp"
#include "FloppyHardware.hpp"
#include <SPIFFS.h>
#include <FS.h>

// Constants
#define FORMAT_SPIFFS_IF_FAILED true
#define AES_BITS 128

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

/*uint8_t FloppyFiles::readFileToSerial(const char *filename)
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
}*/

uint8_t FloppyFiles::readFileToSerial(const char *filename)
{
  // Temp AES key and IV
  byte aes_key[16] = {0};
  byte aes_iv[16] = {0};

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

  // Read the IV from the file
  file.readBytes((char*)aes_iv, 16);

  // Read encrypted data from the file into a buffer
  char encryptedData[256] = {0};
  size_t bytesRead = file.readBytes(encryptedData, sizeof(encryptedData) - 1);
  file.close();
  
  if (bytesRead == 0)
  {
    FloppyHardware::print("Error: File is empty or could not be read\n");
    return 0;
  }

  // Decrypt the data
  byte decrypted[256] = {0};
  aesLib.decrypt64(encryptedData, bytesRead, decrypted, aes_key, AES_BITS, aes_iv);

  // Initialize CRC32
  unsigned long crc = 0xFFFFFFFF;

  // Send decrypted data to the serial port and calculate CRC32
  for (size_t i = 0; i < strlen((char*)decrypted); i++)
  {
    byte data = decrypted[i];
    Serial.write(data);
    crc ^= data;

    // Compute CRC32
    for (int j = 0; j < 8; j++)
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

  // Send end-of-file marker
  FloppyHardware::print("\x04"); // End of transmission
  FloppyHardware::print("\n");

  // Finalize CRC32
  crc = ~crc;

  // Print computed CRC32
  char crcStr[10];
  sprintf(crcStr, "%08X", crc); // Convert to hexadecimal string (8 digits)
  FloppyHardware::print(crcStr);
  FloppyHardware::print("\n");

  return 1;
}

uint8_t FloppyFiles::writeFileFromSerial(const char *filename)
{
  // Temp AES key and IV
  byte aes_key[16] = {0};
  byte aes_iv[16] = {0};

  // Open the file
  File file = SPIFFS.open(filename, "w");
  if (!file)
  {
    FloppyHardware::print("Error: File could not be created\n");
    return 0;
  }

  // Write iv to the file
  file.write(aes_iv, 16);

  // Initialize CRC32
  unsigned long crc = 0xFFFFFFFF;

  // Read data from the serial port into a plain text buffer
  char plainData[256] = {0};
  size_t index = 0;

  while (true)
  {
    while (!FloppyHardware::available())
    {
      delay(1);
    }

    char data = FloppyHardware::read();

    // Check for end-of-file marker
    if (data == 0x04 || index >= sizeof(plainData) - 1)
    {
      break;
    }

    plainData[index++] = data;
    crc ^= data;

    // Compute CRC32
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

  // Null-terminate the plain data buffer
  plainData[index] = '\0';

  // Finalize CRC32
  crc = ~crc;

  // Print computed CRC32
  char crcStr[10];
  sprintf(crcStr, "%08X", crc); // Convert to hexadecimal string (8 digits)
  FloppyHardware::print(crcStr);
  FloppyHardware::print("\n");

  // Encrypt the data
  char encrypted[256] = {0};
  aesLib.encrypt64((const byte*)plainData, strlen(plainData), encrypted, aes_key, AES_BITS, aes_iv);

  // Write encrypted data to the file
  file.write((uint8_t *)encrypted, strlen(encrypted));
  file.close();

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