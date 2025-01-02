// Includes
#include "FloppyDisplay.hpp"

// Functions
FloppyDisplay::FloppyDisplay(FloppyDisplay::DisplayDriverType driver, uint8_t bl, uint8_t mosi, uint8_t clk, uint8_t dc, uint8_t rst, uint8_t cs, uint32_t spiFrequency)
{
  // Initialize the driver
  switch (driver)
  {
  case FloppyDisplay::DisplayDriverType::ST7789_V2:
#ifdef INCLUDE_ST7789_V2
    this->driver = new ST7789_V2(bl, mosi, clk, dc, rst, cs, spiFrequency);
#endif
    break;

  default:
    break;
  }
}

void FloppyDisplay::init(void)
{
  assert(this->driver != nullptr);

  // Initialize the screen
  this->driver->initScreen();

// Init the canvas
#ifdef USE_CANVAS
  this->canvas = (uint8_t *)FloppyHardware::malloc(this->driver->getScreenWidth() * this->driver->getScreenHeight() * 3 * sizeof(uint8_t));

  // Clear the canvas
  this->clearCanvas();
#endif
}

uint16_t FloppyDisplay::getScreenWidth(void)
{
  assert(this->driver != nullptr);
  return this->driver->getScreenWidth();
}

uint16_t FloppyDisplay::getScreenHeight(void)
{
  assert(this->driver != nullptr);
  return this->driver->getScreenHeight();
}

void FloppyDisplay::turnOn(void)
{
  assert(this->driver != nullptr);
  this->driver->turnOn();
}

void FloppyDisplay::turnOff(void)
{
  assert(this->driver != nullptr);
  this->driver->turnOff();
}

void FloppyDisplay::setBrightness(uint8_t brightness)
{
  assert(this->driver != nullptr);
  this->driver->setBrightness(brightness);
}

uint8_t FloppyDisplay::getBrightness(void)
{
  assert(this->driver != nullptr);
  return this->driver->getBrightness();
}

void FloppyDisplay::clearScreen(void)
{
  assert(this->driver != nullptr);
  uint8_t *pixels = (uint8_t *)FloppyHardware::malloc(this->driver->getScreenWidth() * this->driver->getScreenHeight() * 3 * sizeof(uint8_t));
  memset(pixels, 0, this->driver->getScreenWidth() * this->driver->getScreenHeight() * 3);
  this->driver->drawRectangle(0, 0, this->driver->getScreenWidth(), this->driver->getScreenHeight(), pixels);
}

void FloppyDisplay::drawRectangle(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels)
{
  assert(this->driver != nullptr);
  this->driver->drawRectangle(x, y, width, height, pixels);
}

void FloppyDisplay::drawCanvas(void)
{
#ifdef USE_CANVAS
  assert(this->driver != nullptr);

  // Draw the canvas
  this->driver->drawRectangle(0, 0, this->driver->getScreenWidth(), this->driver->getScreenHeight(), (uint8_t *)this->canvas);
#endif
}

void FloppyDisplay::clearCanvas(void)
{
#ifdef USE_CANVAS
  assert(this->driver != nullptr);

  // Clear the canvas
  memset(this->canvas, 0, this->driver->getScreenWidth() * this->driver->getScreenHeight() * 3);
#endif
}

void FloppyDisplay::drawRectangleCanvas(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b)
{
#ifdef USE_CANVAS
  assert(this->driver != nullptr);

  // Set the data on the canvas
  for (int i = 0; i < width * height; i++)
  {
    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3] = r;
    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3 + 1] = g;
    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3 + 2] = b;
  }

#endif
}

void FloppyDisplay::drawRectangleCanvas(uint_fast16_t x, uint_fast16_t y, uint_fast16_t width, uint_fast16_t height, uint8_t *pixels)
{
#ifdef USE_CANVAS
  assert(this->driver != nullptr);

  // Set the data on the canvas
  for (int i = 0; i < width * height; i++)
  {
    // If the color is black make the pixel transparent
    if (pixels[i * 3] == 0 && pixels[i * 3 + 1] == 0 && pixels[i * 3 + 2] == 0)
      continue;

    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3] = pixels[i * 3];
    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3 + 1] = pixels[i * 3 + 1];
    this->canvas[(y + i / width) * this->driver->getScreenWidth() * 3 + (x + i % width) * 3 + 2] = pixels[i * 3 + 2];
  }

#endif
}

void FloppyDisplay::drawStringCanvas(int x, int y, const char *string, uint8_t r, uint8_t g, uint8_t b, uint16_t size, uint16_t angle)
{
  assert(x >= 0 && y >= 0 && x < this->driver->getScreenWidth() && y < this->driver->getScreenHeight() && string != nullptr && angle % 90 == 0);

  const uint16_t CHAR_SIZE = 8;

  // Get the string length
  int stringLength = strlen(string);

  // Get the string
  char *stringCopy = (char *)FloppyHardware::malloc(stringLength * sizeof(char));

  // If the angle is 180 or 270, we need to invert the string
  if (angle == 180 || angle == 270)
  {
    for (int i = 0; i < stringLength; i++)
    {
      stringCopy[i] = string[stringLength - i - 1];
    }
  }
  else
  {
    strcpy(stringCopy, string);
  }

  // Draw the characters
  for (int i = 0; i < stringLength; i++)
  {

    // If angle is 90, the x and y are inverted
    if (angle == 90)
    {
      // Check if the char is not in the canvas
      if (y + (i + 1) * size > this->driver->getScreenHeight() || x - size < 0)
        break;

      // Draw the char on the canvas
      this->drawCharCanvas(x - size, y + i * size, string[i], r, g, b, size, angle);
    }
    // If angle is 180
    else if (angle == 180)
    {
      // Check if the char is not in the canvas
      if (x - (i + 1) * size < 0 || y - size < 0)
        break;

      // Draw the char on the canvas
      this->drawCharCanvas(x - (i + 1) * size, y - size, string[i], r, g, b, size, angle);
    }
    // If angle is 270, the x and y are inverted
    else if (angle == 270)
    {
      // Check if the char is not in the canvas
      if (y - (i + 1) * size < 0 || x - size < 0)
        break;

      // Draw the char on the canvas
      this->drawCharCanvas(x - size, y - (i + 1) * size, string[i], r, g, b, size, angle);
    }
    // If angle is 0
    else
    {
      // Check if the char is not in the canvas
      if (x + (i + 1) * size > this->driver->getScreenWidth())
        break;

      // Draw the char on the canvas
      this->drawCharCanvas(x + i * size, y, string[i], r, g, b, size, angle);
    }
  }
}

uint16_t FloppyDisplay::getStringWidth(const char *string, uint16_t size)
{
  return strlen(string) * size;
}

uint8_t *FloppyDisplay::rotateImage(uint8_t *image_data, int width, int height, bool clockwise)
{
  assert(image_data != nullptr && width > 0 && height > 0);

  // Allocate the rotated data
  uint8_t *rotated_data = (uint8_t *)FloppyHardware::malloc(width * height * 3 * sizeof(uint8_t));

  if (clockwise)
  {
    // Clockwise rotation
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        int src_index = (y * width + x) * 3;
        int dest_index = (x * height + (height - y - 1)) * 3;
        rotated_data[dest_index] = image_data[src_index];
        rotated_data[dest_index + 1] = image_data[src_index + 1];
        rotated_data[dest_index + 2] = image_data[src_index + 2];
      }
    }
  }
  else
  {
    // Anti-clockwise rotation
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        int src_index = (y * width + x) * 3;
        int dest_index = ((width - x - 1) * height + y) * 3;
        rotated_data[dest_index] = image_data[src_index];
        rotated_data[dest_index + 1] = image_data[src_index + 1];
        rotated_data[dest_index + 2] = image_data[src_index + 2];
      }
    }
  }

  return rotated_data;
}

uint8_t *FloppyDisplay::upscaleImage(uint8_t *image_data, uint16_t width, uint16_t height, uint8_t multiplier)
{
  assert(image_data != nullptr && width > 0 && height > 0 && multiplier > 0);

  // Calculate new dimensions after upscaling
  uint16_t new_width = width * multiplier;
  uint16_t new_height = height * multiplier;

  // Allocate memory for the upscaled image
  uint8_t *upscaled_image = new uint8_t[new_width * new_height * 3];

  // Upscale the image
  for (uint16_t y = 0; y < height; ++y)
  {
    for (uint16_t x = 0; x < width; ++x)
    {
      uint16_t src_index = (y * width + x) * 3;
      uint8_t pixel_r = image_data[src_index];
      uint8_t pixel_g = image_data[src_index + 1];
      uint8_t pixel_b = image_data[src_index + 2];

      // Upscale each pixel
      for (uint8_t i = 0; i < multiplier; ++i)
      {
        for (uint8_t j = 0; j < multiplier; ++j)
        {
          uint16_t dest_index = ((y * multiplier + i) * new_width + (x * multiplier + j)) * 3;
          upscaled_image[dest_index] = pixel_r;
          upscaled_image[dest_index + 1] = pixel_g;
          upscaled_image[dest_index + 2] = pixel_b;
        }
      }
    }
  }

  return upscaled_image;
}

void FloppyDisplay::drawCharCanvas(int x, int y, const char charToDraw, uint8_t r, uint8_t g, uint8_t b, uint16_t size, uint16_t angle)
{
  // Get the character raw data
  const uint8_t *charRawData = this->pixelBase.getCharData(charToDraw);

  // Allocate the char array Get the char data
  bool *initCharData = this->pixelBase.getCharBoolArray(charRawData, 8);

  // Rotate the init char data
  if (angle != 0)
    this->pixelBase.rotateArray(initCharData, angle);

  // Upscale the char data
  bool *charData = this->pixelBase.upscaleCharBoolArray(initCharData, 8, (uint16_t)(size / 8));

  // Allocate the pixels
  uint8_t *pixels = (uint8_t *)FloppyHardware::malloc(size * size * 3 * sizeof(uint8_t));

  // Set the pixels
  for (int i = 0; i < size * size; i++)
  {
    pixels[i * 3] = charData[i] ? r : 0;
    pixels[i * 3 + 1] = charData[i] ? g : 0;
    pixels[i * 3 + 2] = charData[i] ? b : 0;
  }

  // Draw the char on the canvas
  this->drawRectangleCanvas(x, y, size, size, pixels);

  // Free the memory
  FloppyHardware::free(pixels);
  FloppyHardware::free(charData);
  FloppyHardware::free(initCharData);
}