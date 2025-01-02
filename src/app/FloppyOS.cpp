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

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
  }

  Serial.println("SPIFFS mounted successfully");

  // Initialize the buttons
  FloppyGPIO::setPinMode(BUTTON_A, INPUT_PULLUP);

  // Return
  return 0;
}
uint8_t FloppyOS::update(void)
{
  // Read the command
  FloppyOSCommands::readCommand(this->display);

  // Return
  return 0;
}
