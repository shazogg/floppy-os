// Includes
#include "FloppyOSCommands.hpp"
#include "FloppyHardware.hpp"
#include <cstring>
#include "FloppyFiles.hpp"
#include "FloppyOS.hpp"

// Constructor
FloppyOSCommands::FloppyOSCommands()
{
}

// Functions
void FloppyOSCommands::readCommand(FloppyDisplay *display)
{
  if (FloppyHardware::available() > 0)
  {
    char *line = FloppyHardware::readLine();

    // compare the line
    if (strcmp(line, "read_file") == 0)
    {
      // Read file path
      const char *line = FloppyHardware::readLine();
      char filename[255];

      // Copy the string safely
      strncpy(filename, line, sizeof(filename) - 1);
      filename[sizeof(filename) - 1] = '\0'; // Ensure the termination by '\0'

      // Acknowledge the command TODO: change if file is authorized
      FloppyHardware::print("read_file_authorized\n");

      // Read the file
      FloppyOS::getInstance()->getFiles()->readFileToSerial(filename);
    }
    else if (strcmp(line, "write_file") == 0)
    {
      // Read file path
      const char *line = FloppyHardware::readLine();
      char filename[255];

      // Copy the string safely
      strncpy(filename, line, sizeof(filename) - 1);
      filename[sizeof(filename) - 1] = '\0'; // Ensure the termination by '\0'

      // Read the file size
      uint32_t fileSize = atoi(FloppyHardware::readLine());

      // Check if enough space
      if (FloppyFiles::getFreeSpace() < fileSize)
      {
        FloppyHardware::print("write_file_not_enough_storage\n");
        return;
      }

      // Acknowledge the command TODO: change if file is authorized
      FloppyHardware::print("write_file_authorized\n");

      // Write the file
      uint8_t writeResult = FloppyOS::getInstance()->getFiles()->writeFileFromSerial(filename);

      // Print the result
      if (writeResult == 0)
      {
        FloppyHardware::print("write_file_success\n");
      }
      else
      {
        FloppyHardware::print("write_file_error\n");
      }
    }
  }
}