// Includes
#include "FloppyOS.hpp"
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <cstring>
#include "FloppyOSCommands.hpp"

// Defines
#define DISPLAY_MOSI 15
#define DISPLAY_CLK 13
#define DISPLAY_DC 14
#define DISPLAY_RST 12
#define DISPLAY_CS 5
#define DISPLAY_BL 27

// Buttons
#define BUTTON_A 37

// Constants
const bool DEBUG = false;

// Variables
FloppyOS *FloppyOS::singleton = nullptr; // Initialize the instance

// Constructor
FloppyOS::FloppyOS(void)
{
  this->display = new FloppyDisplay(FloppyDisplay::DisplayDriverType::ST7789_V2, DISPLAY_BL, DISPLAY_MOSI, DISPLAY_CLK, DISPLAY_DC, DISPLAY_RST, DISPLAY_CS);
  this->files = new FloppyFiles();
  this->commands = new FloppyOSCommands();
}

// Functions
FloppyOS *FloppyOS::getInstance()
{
  if (singleton == nullptr)
  {
    singleton = new FloppyOS();
  }
  return singleton;
}

FloppyDisplay *FloppyOS::getDisplay(void)
{
  return this->display;
}

FloppyFiles *FloppyOS::getFiles(void)
{
  return this->files;
}

uint8_t FloppyOS::setup(void)
{
  // Hold power on
  FloppyGPIO::setPinMode(4, OUTPUT);
  FloppyGPIO::setPinDigitalValue(4, HIGH);

  // Initialize the display
  (*this->display).init();

  // Clear the screen
  (*this->display).clearScreen();

  delay(250);

  // Set backlight brightness
  (*this->display).setBrightness(128);

  // Draw the splash screen
  this->drawSplashScreen();

  // Draw debug lines
  if (DEBUG)
  {
    (*this->display).drawRectangleCanvas(0, (*this->display).getScreenHeight() / 2, (*this->display).getScreenWidth(), 1, 255, 0, 0);
    (*this->display).drawRectangleCanvas((*this->display).getScreenWidth() / 2, 0, 1, (*this->display).getScreenHeight(), 0, 255, 0);

    // Draw the canvas
    (*this->display).drawCanvas();
  }

  // Initialize file system
  if (!FloppyFiles::initFS())
  {
    FloppyHardware::print("file_system_mount_failed\n");

    // Show error on screen
    (this->display)->clearCanvas();
    (this->display)->drawStringCanvas(127, 8, "error: File system mount failed", 255, 0, 0, 8, 90);
    (this->display)->drawCanvas();

    return 0;
  }

  // Initialize settings
  if (!this->files->initSettings())
  {
    FloppyHardware::print("settings_init_failed\n");

    // Show error on screen
    (this->display)->clearCanvas();
    (this->display)->drawStringCanvas(127, 8, "error: Settings init failed", 255, 0, 0, 8, 90);
    (this->display)->drawCanvas();

    return 0;
  }

  // Return
  return 1;
}
uint8_t FloppyOS::update(void)
{
  // Read the command
  this->commands->readCommand(this->display);

  // Return
  return 0;
}
