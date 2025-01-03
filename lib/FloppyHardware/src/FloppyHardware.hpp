// Floppy Hardware Library
#pragma once

// Includes
#include <cstdint>
#include <cstddef>
#include <Arduino.h>

// Main class
class FloppyHardware
{
public:
  // Functions

  /**
   * @brief Check if the PSRAM is available
   */
  static bool checkPSRAM(void);

  /**
   * @brief Allocate memory
   *
   * @param size The size of the memory to allocate
   */
  static void *malloc(size_t size);

  /**
   * @brief Free memory
   *
   * @param ptr The pointer to free
   */
  static void free(void *ptr);

  /**
   * @brief Initialize the serial port
   *
   * @param baudrate The baudrate
   */
  static void initSerial(uint32_t baudrate);

  /**
   * @brief Print a message
   *
   * @param message The message to print
   */
  static void print(const char *message);

  /**
   * @brief Print a formated message
   *
   * @param message The message to print
   * @param ... The arguments
   */
  static void printf(const char *format, ...);

  /**
   * @brief Check if there are characters available in the serial port
   *
   * @return The number of characters available
   */
  static uint8_t available();

  /**
   * @brief Read a character from the serial port
   *
   * @return The character read
   */
  static char read();

  /**
   * @brief Read a line from the serial port
   *
   * @return The line read
   */
  static char *readLine(void);
};