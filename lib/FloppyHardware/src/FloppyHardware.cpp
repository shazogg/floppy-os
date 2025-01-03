// Includes
#include "FloppyHardware.hpp"

// Constants
#define SERIAL_BUFFER_SIZE 256

// Functions
bool FloppyHardware::checkPSRAM(void)
{
  return psramInit();
}

void *FloppyHardware::malloc(size_t size)
{
#if defined(BOARD_HAS_PSRAM) && !defined(CONFIG_SPIRAM_USE_MALLOC)
  return ps_malloc(size);
#else
  return ::malloc(size);
#endif
}

void FloppyHardware::free(void *ptr)
{
  ::free(ptr);
}

void FloppyHardware::initSerial(uint32_t baudrate)
{
  Serial.begin(baudrate);
}

void FloppyHardware::print(const char *message)
{
  Serial.print(message);
}

void FloppyHardware::printf(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

uint8_t FloppyHardware::available()
{
  return Serial.available();
}

char FloppyHardware::read()
{
  return Serial.read();
}

char* FloppyHardware::readLine() {
  // Allocate memory for a string
  char *buffer = (char *)malloc(SERIAL_BUFFER_SIZE);
  uint16_t index = 0;

  while (true) {
    // Read one byte from the serial port
    while (!FloppyHardware::available()) {
      delay(1);
    }
    char data = FloppyHardware::read();

    // Check for the end of line marker
    if (data == '\n' || data == '\r' || index == SERIAL_BUFFER_SIZE - 1) {
      buffer[index] = '\0';
      return buffer;
    }

    // Store the character
    buffer[index++] = data;
  }

  return nullptr;
}