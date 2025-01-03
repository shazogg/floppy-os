// Includes
#include "app/FloppyOS.hpp"
#include <FloppyGPIO.hpp>
#include <FloppyDisplay.hpp>
#include <FloppyHardware.hpp>
#include <Arduino.h>

// Variables
FloppyOS *floppyOSInstance = nullptr;
uint8_t setupSuccess = 0;

// Setup function
void setup(void)
{
  // Initialize the serial port
  FloppyHardware::initSerial(115200);

  // Exit if the PSRAM is not available
  if (!FloppyHardware::checkPSRAM())
  {
    FloppyHardware::print("PSRAM not available\n");
    return;
  }

  // Create the application
  floppyOSInstance = FloppyOS::getInstance();

  // Setup the application
  setupSuccess = floppyOSInstance->setup();
}

// Main loop
void loop()
{
  // Update the application
  
  if (setupSuccess)
  {
    floppyOSInstance->update();
  }
}
