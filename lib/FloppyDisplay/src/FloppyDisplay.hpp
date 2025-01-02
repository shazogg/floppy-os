// Floppy Display Library
#pragma once

// Define USE_CANVAS to use the canvas
#define USE_CANVAS

// Includes
#include <cstdint>
#include "drivers/DisplayDriver.hpp"
#include "fonts/BaseFont.hpp"
#include "fonts/PixelBase.hpp"
#include <FloppyHardware.hpp>

#define INCLUDE_ST7789_V2 #Include ST7789V2 driver

// Include selected screens
#ifdef INCLUDE_ST7789_V2
#include "drivers/ST7789_V2.hpp"
#endif

// Main class
class FloppyDisplay
{
public:
  /**
   * @brief The display driver type
   *
   */
  enum class DisplayDriverType
  {
    ST7789_V2, // ST7789 V2 driver
    NONE       // No driver, used for testing
  };

  // Constructor
  FloppyDisplay(DisplayDriverType driverType, uint8_t bl, uint8_t mosi, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t spiFrequency = 32000000);

  /**
   * @brief Initialize the screen
   * @pre The display driver must be initialized
   * @note The canvas is initialized if USE_CANVAS is defined and it will use 3 bytes per pixel * width * height
   *
   */
  void init(void);

  /**
   * @brief Get the Screen Width object
   * @pre The display driver must be initialized
   *
   * @return The screen width
   */
  uint16_t getScreenWidth(void);

  /**
   * @brief Get the Screen Height object
   * @pre The display driver must be initialized
   *
   * @return The screen height
   */
  uint16_t getScreenHeight(void);

  /**
   * @brief Turn on the screen
   *
   */
  void turnOn(void);

  /**
   * @brief Turn off the screen
   *
   */
  void turnOff(void);

  /**
   * @brief Set the brightness of the screen
   * @pre The display driver must be initialized
   *
   * @param brightness The brightness
   */
  void setBrightness(uint8_t brightness);

  /**
   * @brief Get the brightness of the screen
   * @pre The display driver must be initialized
   *
   * @return The brightness of the screen
   */
  uint8_t getBrightness(void);

  /**
   * @brief Clear the screen
   *
   */
  void clearScreen(void);

  /**
   * @brief Fill the screen with a color
   * @pre The display driver must be initialized
   *
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   */
  void fillScreen(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b);

  /**
   * @brief Draw a pixel on the screen
   * @pre The display driver must be initialized
   *
   * @param x The x position
   * @param y The y position
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   */
  void drawPixel(uint_fast16_t x, uint_fast16_t y, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b);

  /**
   * @brief Draw a rectangle on the screen
   * @pre The display driver must be initialized
   *
   * @param x The x position
   * @param y The y position
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   */
  void drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b);

  /**
   * @brief Draw a rectangle on the screen
   * @pre The display driver must be initialized
   *
   * @param x The x position
   * @param y The y position
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   * @param pixels The pixels to draw
   */
  void drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels);

  /**
   * @brief Draw the canvas
   * @pre The display driver must be initialized
   * @note The canvas must be enabled and color black is transparent
   *
   */
  void drawCanvas(void);

  /**
   * @brief Clear the canvas
   *
   */
  void clearCanvas(void);

  /**
   * @brief Draw a rectangle on the canvas
   * @note The canvas must be enabled and color black is transparent
   *
   * @param x The x position
   * @param y The y position
   * @param width The width
   * @param height The height
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   */
  void drawRectangleCanvas(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b);

  /**
   * @brief Draw a rectangle on the canvas
   * @note The canvas must be enabled and color black is transparent
   *
   * @param x The x position
   * @param y The y position
   * @param width The width
   * @param height The height
   * @param pixels The pixels
   */
  void drawRectangleCanvas(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels);

  /**
   * @brief Draw a string on the canvas
   * @note The canvas must be enabled and color black is transparent
   *
   * @param x The x position
   * @param y The y position
   * @param charToDraw The character to draw
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   * @param size The size of the character
   * @param angle The angle of the character
   */
  void drawStringCanvas(int x, int y, const char *string, uint8_t r, uint8_t g, uint8_t b, uint16_t size, uint16_t angle = 0);

  /**
   * @brief Get the String Width object
   *
   * @param string The string
   * @param size The size of the character
   * @return The width of the string
   */
  uint16_t getStringWidth(const char *string, uint16_t size);

  /**
   * @brief Rotate an image
   * @pre Image data != nullptr and width > 0 and height > 0
   *
   * @param image_data The image data
   * @param width The width of the image
   * @param height The height of the image
   * @param clockwise Clockwise or counter clockwise
   * @return The rotated image
   */
  uint8_t *rotateImage(uint8_t *image_data, int width, int height, bool clockwise = true);

  /**
   * @brief Upscale an image
   * @pre Image data != nullptr and width > 0 and height > 0
   *
   * @param image_data The image data
   * @param width The width of the image
   * @param height The height of the image
   * @param upscale_factor The upscale factor
   * @return The upscaled image
   */
  uint8_t *upscaleImage(uint8_t *image_data, uint16_t width, uint16_t height, uint8_t multiplier);

private:
  // Variables
  DisplayDriver *driver; // The display driver

  uint8_t *canvas = nullptr; // The canvas

  // Fonts
  PixelBase pixelBase; // Pixel base font

  // Functions
  /**
   * @brief Draw a character on the canvas
   * @note The canvas must be enabled and color black is transparent
   *
   * @param x The x position
   * @param y The y position
   * @param charToDraw The character to draw
   * @param charWidth The character width
   * @param charHeight The character height
   * @param r The red color
   * @param g The green color
   * @param b The blue color
   * @param size The size of the character
   * @param angle The angle of the character
   */
  void drawCharCanvas(int x, int y, const char charToDraw, uint8_t r, uint8_t g, uint8_t b, uint16_t size, uint16_t angle = 0);
};
