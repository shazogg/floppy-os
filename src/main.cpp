// Includes
#include "app/FloppyOS.hpp"
#include <FloppyGPIO.hpp>
#include <FloppyDisplay.hpp>
#include <FloppyHardware.hpp>
#include <Arduino.h>

// Variables
FloppyOS *FloppyOS = nullptr;

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
  FloppyOS = FloppyOS::getInstance();

  // Setup the application
  FloppyOS->setup();
}

// Main loop
void loop()
{
  // Update the application
  FloppyOS->update();
}
