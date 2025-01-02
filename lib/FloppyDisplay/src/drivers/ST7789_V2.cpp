// Includes
#include "ST7789_V2.hpp"

// Screen SPI mode
#define ST7789_V2_SPI_MODE 3
#define ST7789_V2_SPI_FREQUENCY 40000000

// Screen offsets and sizes
#define TFT_X_OFFSET 52
#define TFT_Y_OFFSET 40
#define TFT_WIDTH 135
#define TFT_HEIGHT 240

// Screen commands adresses
#define ST7789_SWRESET 0x01 // Software reset
#define ST7789_RAMCTRL 0xB0 // RAM control
#define ST7789_IDMOFF 0x38  // Idle mode off
#define ST7789_SLPIN 0x10   // Sleep in
#define ST7789_SLPOUT 0x11  // Sleep out
#define ST7789_DISPOFF 0x28 // Display off
#define ST7789_DISPON 0x29  // Display on
#define ST7789_INVOFF 0x20  // Display inversion off
#define ST7789_INVON 0x21   // Display inversion on
#define ST7789_CASET 0x2A   // Column address set
#define ST7789_RASET 0x2B   // Row address set
#define ST7789_RAMWR 0x2C   // RAM write
#define ST7789_NORON 0x13   // Normal display mode on

// Functions

ST7789_V2::ST7789_V2(uint8_t bl, uint8_t mosi, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t spiFrequency)
{
  this->bl = bl;
  this->mosi = mosi;
  this->clk = clk;
  this->dc = dc;
  this->rst = rst;
  this->cs = cs;
  this->spiFrequency = spiFrequency;
  this->backlightBrightness = 0;

  // Create the SPI object
  this->spi = new FloppySPI(this->mosi, -1, this->clk, this->dc, this->rst, this->cs, this->spiFrequency);
}

void ST7789_V2::initScreen(void)
{
  // Set pins mode
  FloppyGPIO::setPinMode(this->bl, OUTPUT);

  // Begin SPI
  this->spi->begin();

  // Hard reset
  this->spi->hardReset();

  // Send init commands

  // Software reset
  this->spi->sendData(ST7789_SWRESET);
  delay(130);

  // RAM control
  uint8_t ramctrl_arguments[] = {0x00, 0xC0};
  this->spi->sendData(ST7789_RAMCTRL, ramctrl_arguments, 2);

  // Invert screen color
  this->spi->sendData(ST7789_INVON);

  // Normal mode
  this->spi->sendData(ST7789_NORON);

  // Exit sleep mode
  this->spi->sendData(ST7789_SLPOUT);
  delay(130);

  // Idle mode off
  this->spi->sendData(ST7789_IDMOFF);

  // Display on
  this->spi->sendData(ST7789_DISPON);
}

uint16_t ST7789_V2::getScreenWidth(void)
{
  return TFT_WIDTH;
}

uint16_t ST7789_V2::getScreenHeight(void)
{
  return TFT_HEIGHT;
}

void ST7789_V2::turnOn(void)
{
  // Exit sleep mode
  this->spi->sendData(ST7789_SLPOUT);
  delay(130);

  // Enable backlight
  FloppyGPIO::setPinAnalogValue(this->bl, this->backlightBrightness);
}

void ST7789_V2::turnOff(void)
{
  // Disable backlight
  FloppyGPIO::setPinAnalogValue(this->bl, 0);

  // Enter sleep mode
  this->spi->sendData(ST7789_SLPIN);
  delay(5);
}

void ST7789_V2::setBrightness(uint16_t brightness)
{
  this->backlightBrightness = brightness;
  FloppyGPIO::setPinAnalogValue(this->bl, this->backlightBrightness);
}

uint16_t ST7789_V2::getBrightness(void)
{
  return this->backlightBrightness;
}

void ST7789_V2::drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels)
{
  setWindow(x, y, x + width, y + height);

  // Set write command
  this->spi->sendData(ST7789_RAMWR, pixels, width * height * 3);
}

void ST7789_V2::setWindow(uint_fast16_t xs, uint_fast16_t ys, uint_fast16_t xe, uint_fast16_t ye)
{
  assert(xs > 0 && xs < xe && xe < TFT_WIDTH && ys > 0 && ys < ye && ye < TFT_HEIGHT);

  // Add offsets
  xs += TFT_X_OFFSET;
  xe += TFT_X_OFFSET - 1;
  ys += TFT_Y_OFFSET;
  ye += TFT_Y_OFFSET - 1;

  // Set column offset and size
  uint8_t caset_arguments[] = {(uint8_t)(xs >> 8), (uint8_t)(xs & 0xFF), (uint8_t)(xe >> 8), (uint8_t)(xe & 0xFF)};
  this->spi->sendData(ST7789_CASET, caset_arguments, 4);

  // Set row offset and size
  uint8_t raset_arguments[] = {(uint8_t)(ys >> 8), (uint8_t)(ys & 0xFF), (uint8_t)(ye >> 8), (uint8_t)(ye & 0xFF)};
  this->spi->sendData(ST7789_RASET, raset_arguments, 4);
}