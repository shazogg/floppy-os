// Includes
#include <cstdint>
#include "BaseFont.hpp"
#include <FloppyHardware.hpp>
#include <Arduino.h>

// Functions
bool *BaseFont::getCharBoolArray(const uint8_t *charData, uint16_t size)
{
  // Allocate the array of the size of the character
  bool *boolArray = (bool *)FloppyHardware::malloc(size * size * sizeof(bool));

  // Convert the character data to a boolean array
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      boolArray[i * size + j] = (charData[i] >> (size - 1 - j)) & 1;
    }
  }
  return boolArray;
}

bool *BaseFont::upscaleCharBoolArray(const bool *charData, uint16_t initialSize, uint16_t multiplier)
{
  // Allocate the array of the upscaled size
  uint16_t upscaledSize = initialSize * multiplier;
  bool *boolArray = (bool *)FloppyHardware::malloc(upscaledSize * upscaledSize * sizeof(bool));

  // Set all the values to false
  memset(boolArray, 0, upscaledSize * upscaledSize * sizeof(bool));

  // Upscale the character data
  for (int y = 0; y < upscaledSize; y++)
  {
    for (int x = 0; x < upscaledSize; x++)
    {
      // Calculate the corresponding position in the original charData
      int originalX = x / multiplier;
      int originalY = y / multiplier;

      // If the original pixel is true, set the upscaled pixel to true
      if (charData[originalY * initialSize + originalX])
      {
        boolArray[y * upscaledSize + x] = true;
      }
    }
  }
  return boolArray;
}