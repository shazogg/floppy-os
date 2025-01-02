// Display driver abstraction
#pragma once

// Includes
#include <cstdint>

// Main class prototype
class DisplayDriver
{
public:
  /**
   * @brief Initialize the screen
   *
   */
  virtual void initScreen(void) = 0;

  /**
   * @brief Get the Screen Width object
   *
   * @return The screen width
   */
  virtual uint16_t getScreenWidth(void) = 0;

  /**
   * @brief Get the Screen Height object
   *
   * @return The screen height
   */
  virtual uint16_t getScreenHeight(void) = 0;

  /**
   * @brief Turn on the screen
   *
   */
  virtual void turnOn(void) = 0;

  /**
   * @brief Turn off the screen
   *
   */
  virtual void turnOff(void) = 0;

  /**
   * @brief Set the brightness of the screen
   *
   * @param brightness The brightness
   */
  virtual void setBrightness(uint16_t brightness) = 0;

  /**
   * @brief Get the brightness of the screen
   *
   * @return The brightness of the screen
   */
  virtual uint16_t getBrightness(void) = 0;

  /**
   * @brief Draw a rectangle on the screen
   *
   * @param x The x position
   * @param y The y position
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   * @param pixels The pixels
   */
  virtual void drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels) = 0;

protected:
  uint16_t backlightBrightness;
};