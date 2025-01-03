// Files class
#pragma once

// Includes
#include <cstdint>
#include <Arduino.h>
#include <SPIFFS.h>
#include <Preferences.h>

// Main class prototype
class FloppyFiles
{
public:
  // Constructor
  FloppyFiles();

  /**
   * @brief Initialize the settings
   *
   * @return uint8_t The status of the initialization (0 = ERROR, 1 = OK)
   */
  uint8_t initSettings();

  /**
   * @brief Initialize the file system
   *
   * @return uint8_t The status of the initialization (0 = ERROR, 1 = OK)
   */
  static uint8_t initFS();

  /**
   * @brief List files in a directory
   *
   * @param dir The directory
   */
  // static void listFiles(const char *dir);

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

  /**
   * @brief Set the string object
   *
   * @param key The key
   */
  void setString(const char *key, const char *value);

  /**
   * @brief Read a string from the preferences
   *
   * @param key The key
   * @return The value
   */
  const char *getString(const char *key);

  /**
   * @brief Set the Int object
   * 
   * @param key The key
   * @param value The value
   */
  void setInt(const char *key, uint32_t value);

  /**
   * @brief Read an int from the preferences
   * 
   * @param key The key
   * @return The value
   */
  uint32_t getInt(const char *key);

  /**
   * @brief Set the Char object
   * 
   * @param key The key
   * @param value The value
   */
  void setChar(const char *key, char value);

  /**
   * @brief Read a char from the preferences
   * 
   * @param key The key
   * @return The value
   */
  char getChar(const char *key);

  private:
    Preferences preferences;
};