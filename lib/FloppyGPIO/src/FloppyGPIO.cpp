// Includes
#include "FloppyGPIO.hpp"

// Functions
void FloppyGPIO::setPinMode(uint8_t pin, uint8_t mode)
{
  pinMode(pin, mode);
}

void FloppyGPIO::setPinDigitalValue(uint8_t pin, bool value)
{
  digitalWrite(pin, value);
}

bool FloppyGPIO::getPinDigitalValue(uint8_t pin)
{
  return digitalRead(pin);
}

void FloppyGPIO::setPinAnalogValue(uint8_t pin, uint16_t value)
{
  analogWrite(pin, value);
}

uint16_t FloppyGPIO::getPinAnalogValue(uint8_t pin)
{
  return analogRead(pin);
}