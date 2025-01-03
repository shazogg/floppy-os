// Floppy Hardware Library
#pragma once

// Includes
#include <FloppyDisplay.hpp>

// Main class
class FloppyOSCommands
{
public:
  // Constructor
  FloppyOSCommands();

  /**
   * @brief Read a command on serial
   * 
   * @param command The command to execute
   */
  void readCommand(FloppyDisplay *display);
};