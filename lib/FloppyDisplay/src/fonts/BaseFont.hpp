// Base font
#pragma once

// Includes
#include <cstdint>
#include <FloppyHardware.hpp>

class BaseFont
{
public:
  /**
   * @brief Get the Char object
   *
   * @param charToGet The character to get
   * @return An array of 8 bytes representing the character
   */
  virtual const uint8_t *getCharData(const char charToGet) = 0;

  /**
   * @brief Get the Char Bool Array object
   *
   * @param charToGet The character to get
   * @param size The size of the character data
   * @return The character data as a boolean array pointer
   */
  bool *getCharBoolArray(const uint8_t *charData, uint16_t size);


  /**
   * @brief Upscale a character
   * 
   * @param charData The character data
   * @param initialSize The initial size of the character
   * @param multiplier The multiplier
   * @return The upscaled character
   */
  bool *upscaleCharBoolArray(const bool *charData, uint16_t initialSize, uint16_t multiplier);

  /**
   * @brief Rotate an array of 64 elements
   *
   * @param array The array to rotate
   */
  virtual void rotateArray(bool *array, int angle) = 0;
};
