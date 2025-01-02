// SPI class
#pragma once

// Includes
#include <cstdint>
#include <string.h>
#include <FloppyGPIO.hpp>
#include <SPI.h>

// Main class prototype
class FloppySPI
{
public:
  /**
   * @brief Construct a new SPI object
   *
   * @param mosi The MOSI pin
   * @param miso The MISO pin
   * @param clk The CLK pin
   * @param dc The DC pin
   * @param rst The RST pin
   * @param cs The CS pin
   * @param frequency The frequency
   */
  FloppySPI(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t frequency = 1000000);

  /**
   * @brief Start the SPI communication
   *
   */
  void begin(void);

  /**
   * @brief Send data to the SPI
   * @note If data is not null, the command is followed by the data and the length is the length of the data
   *
   * @param command The command adress
   * @param data The data
   * @param length The length of the data
   */
  void sendData(const uint8_t command, uint8_t *data = nullptr, uint32_t length = 0);

  /**
   * @brief Hard reset the SPI
   *
   */
  void hardReset(void);

private:
  // Variables

  // Pins
  uint8_t mosi;
  uint8_t miso;
  uint8_t clk;
  uint8_t dc;
  uint8_t rst;
  uint8_t cs;

  // SPI configuration
  uint32_t maxTransfertSize;
  uint32_t frequency;
};