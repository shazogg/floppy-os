// Files class
#pragma once

// Includes
#include <cstdint>
#include <Arduino.h>

// Main class prototype
class FloppyFiles
{
public:
  /**
   * @brief List files in a directory
   *
   * @param dir The directory
   */
  //static void listFiles(const char *dir);

  /**
   * @brief Get the free space on the storage
   *
   * @return The free space
   */
  static uint32_t getFreeSpace(void);

  /**
   * @brief Read a file to the serial
   *
   * @param filename The filename
   * @return The status of the read (0 = ERROR, 1 = OK)
   */
  static uint8_t readFileToSerial(const char *filename);

  /**
   * @brief Write a file from the serial
   * 
   * @param filename The filename
   * @return uint8_t The status of the write (0 = ERROR, 1 = OK)
   */
  static uint8_t writeFileFromSerial(const char *filename);
};