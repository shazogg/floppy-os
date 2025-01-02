// GPIO class
#pragma once

// Includes
#include <cstdint>
#include <Arduino.h>

// Main class prototype
class FloppyGPIO
{
public:
  /**
   * @brief Set the Pin Mode
   *
   * @param pin The pin
   * @param mode The mode
   */
  static void setPinMode(uint8_t pin, uint8_t mode);

  /**
   * @brief Set the Pin Value
   *
   * @param pin The pin
   * @param value The value
   */
  static void setPinDigitalValue(uint8_t pin, bool value);

  /**
   * @brief Get the Pin Digital Value object
   *
   * @param pin The pin
   * @return true or false
   */
  static bool getPinDigitalValue(uint8_t pin);

  /**
   * @brief Set the Pin Analog Value object
   *
   * @param pin The pin
   * @param value The value
   */
  static void setPinAnalogValue(uint8_t pin, uint16_t value);

  /**
   * @brief Get the Pin Analog Value object
   *
   * @param pin The pin
   * @return The analog value
   */
  static uint16_t getPinAnalogValue(uint8_t pin);
};