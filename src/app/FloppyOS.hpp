// Floppy Hardware Library
#pragma once

// Includes
#include <cstdint>
#include <cstddef>
#include <FloppyGPIO.hpp>
#include <FloppyDisplay.hpp>
#include <FloppyHardware.hpp>
#include <FloppyFiles.hpp>

// Include assets
#include "assets/icon.hpp"

// Defines
#define FLOPPY_OS_VERSION "Proto V2.0.0"

// Main class
class FloppyOS
{
public:
  /**
   * @brief Avoid the copy of the class
   *
   * @param other The other instance
   */
  FloppyOS(FloppyOS &other) = delete;

  /**
   * @brief Avoir creation of the class
   *
   */
  void operator=(const FloppyOS &) = delete;

  /**
   * @brief Get the Instance object
   *
   * @return The instance of the class
   */
  static FloppyOS *getInstance();

  /**
   * @brief Setup the application
   *
   * @return The status of the setup
   */
  uint8_t setup(void);

  /**
   * @brief Main loop of the application
   *
   * @return The status of the update
   */
  uint8_t update(void);

private:
  // Constructor
  FloppyOS(void);

  // Variables
  static FloppyOS *singleton; // The instance of the class

  // Display
  FloppyDisplay *display = nullptr;

  // Functions

  /**
   * @brief Draw the splash screen
   *
   */
  void drawSplashScreen(void);

/**
 * @brief Draw an image on the screen
 * @pre Image data != nullptr and width > 0 and height > 0
 * 
 * @param x The x position
 * @param y The y position
 * @param width The width of the image
 * @param height The height of the image
 * @param image The image data
 * @param scale The scale of the image
 * @param rotation The rotation of the image (0 = 0, 1 = 90, 2 = 180, 3 = 270)
 */
  void drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *image, uint8_t scale = 1, uint8_t rotation = 0);
};