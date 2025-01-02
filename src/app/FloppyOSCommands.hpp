// Floppy Hardware Library
#pragma once

// Includes
#include <FloppyDisplay.hpp>

// Main class
class FloppyOSCommands
{
public:
  /**
   * @brief Read a command on serial
   * 
   * @param command The command to execute
   */
  static void readCommand(FloppyDisplay *display);
};