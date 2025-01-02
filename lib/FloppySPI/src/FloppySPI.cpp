// Includes
#include "FloppySPI.hpp"

// Defines
#define QUEUE_SIZE 1

// Constructor
FloppySPI::FloppySPI(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t frequency)
{
  // Set the pins
  this->mosi = mosi;
  this->miso = miso;
  this->clk = clk;
  this->dc = dc;
  this->rst = rst;
  this->cs = cs;
  this->frequency = frequency;
}

// Functions
void FloppySPI::begin(void)
{
  // Set pins mode
  if (this->mosi != 255)
    FloppyGPIO::setPinMode(this->mosi, OUTPUT);
  if (this->miso != 255)
    FloppyGPIO::setPinMode(this->miso, INPUT);
  FloppyGPIO::setPinMode(this->clk, OUTPUT);
  FloppyGPIO::setPinMode(this->dc, OUTPUT);
  FloppyGPIO::setPinMode(this->rst, OUTPUT);
  FloppyGPIO::setPinMode(this->cs, OUTPUT);

  // Initialize SPI
  SPI.begin(this->clk, -1, this->mosi, this->cs);
}

void FloppySPI::sendData(const uint8_t command, uint8_t *data, uint32_t length)
{
  FloppyGPIO::setPinDigitalValue(this->dc, LOW); // Command mode

  // Create the command tx buffer
  uint8_t command_tx_buffer[1];
  command_tx_buffer[0] = command;

  SPI.beginTransaction(SPISettings(this->frequency, MSBFIRST, SPI_MODE3)); // Start the SPI transaction

  // Send the command
  SPI.writeBytes(command_tx_buffer, 1);

  SPI.endTransaction(); // End the SPI transaction

  // Send arguments
  if (data != nullptr)
  {
    FloppyGPIO::setPinDigitalValue(this->dc, HIGH); // Data mode

    SPI.beginTransaction(SPISettings(this->frequency, MSBFIRST, SPI_MODE3)); // Start the SPI transaction

    // Send the command
    SPI.writeBytes(data, length);

    SPI.endTransaction(); // End the SPI transaction
  }
}

void FloppySPI::hardReset(void)
{
  FloppyGPIO::setPinDigitalValue(this->rst, HIGH);
  delay(100);
  FloppyGPIO::setPinDigitalValue(this->rst, LOW);
  delay(100);
  FloppyGPIO::setPinDigitalValue(this->rst, HIGH);
  delay(100);
}