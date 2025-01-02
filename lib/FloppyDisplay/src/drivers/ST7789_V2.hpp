// ST7789 V2 lcd driver
#pragma once

// Includes
#include <cstdint>
#include "DisplayDriver.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <FloppyGPIO.hpp>
#include <FloppyHardware.hpp>
#include <FloppySPI.hpp>

// Main class prototype
class ST7789_V2 : public DisplayDriver
{
public:
  // Constructor
  ST7789_V2(uint8_t bl, uint8_t mosi, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t spiFrequency = 1000000);

  // Functions prototypes
  /**
   * @brief Initialize the screen
   * @note The brightness is set to 0 to avoid seeing the screen turning on, set it after clearing the screen
   */
  virtual void initScreen(void) override;

  /**
   * @brief Get the Screen Width object
   *
   * @return The screen width
   */
  virtual uint16_t getScreenWidth(void) override;

  /**
   * @brief Get the Screen Height object
   *
   * @return The screen height
   */
  virtual uint16_t getScreenHeight(void) override;

  /**
   * @brief Turn on the screen
   *
   */
  virtual void turnOn(void) override;

  /**
   * @brief Turn off the screen
   *
   */
  virtual void turnOff(void) override;

  /**
   * @brief Set the brightness of the screen
   *
   * @param brightness The brightness
   */
  virtual void setBrightness(uint16_t brightness) override;

  /**
   * @brief Get the brightness of the screen
   *
   * @return The brightness of the screen
   */
  virtual uint16_t getBrightness(void) override;

  /**
   * @brief Draw a rectangle on the screen
   *
   * @param x The x position
   * @param y The y position
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   * @param pixels The pixels of the rectangle
   */
  virtual void drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels) override;

private:
  // Variables

  // Pins
  uint8_t bl, mosi, clk, dc, rst, cs;

  // SPI frequency
  uint32_t spiFrequency;

  // SPI class
  FloppySPI *spi; // SPI class

  // Functions

  /**
   * @brief Set the screen memory window
   *
   * @param xs Start x position
   * @param ys Start y position
   * @param xe End x position
   * @param ye End y position
   * @pre 0< xs < xe < 240 and 0 < ys < ye < 360
   */
  void setWindow(uint_fast16_t xs, uint_fast16_t ys, uint_fast16_t xe, uint_fast16_t ye);
};
