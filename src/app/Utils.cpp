// Includes
#include "FloppyOS.hpp"

// Functions
void FloppyOS::drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *image, uint8_t scale, uint8_t rotation)
{
  // Allocate the image
  uint8_t *image_data = (uint8_t *)FloppyHardware::malloc(width * height * 3 * sizeof(uint8_t));

  // Copy the image
  memcpy(image_data, image, width * height * 3);

  // Rotate the image
  uint8_t *rotated_image = (*this->display).rotateImage(image_data, width, height, true);

  // Free the memory at image_data
  FloppyHardware::free(image_data);

  // Upscale the image
  uint8_t *upscaled_image = (*this->display).upscaleImage(rotated_image, width, height, scale);

  // Free the memory at rotated_image
  FloppyHardware::free(rotated_image);

  // Draw the image
  (*this->display).drawRectangleCanvas(x, y, width * scale, height * scale, upscaled_image);
}