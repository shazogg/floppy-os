// Includes
#include "FloppyOS.hpp"

// Functions

void FloppyOS::drawSplashScreen(void)
{
  // Clear the canvass
  (*this->display).clearCanvas();

  // Draw the title on the center
  const char *name = "FLOPPY OS";
  uint16_t nameWidth = (*this->display).getStringWidth(name, 16);
  (*this->display).drawStringCanvas((uint16_t)((*this->display).getScreenWidth() / 2) + (16 / 2), ((*this->display).getScreenHeight() / 2) - (nameWidth / 2) + 16, name, 255, 255, 0, 16, 90);

  // Draw the version on the bottom right
  const char *version = FLOPPY_OS_VERSION;
  uint16_t versionWidth = (*this->display).getStringWidth(version, 8);
  (*this->display).drawStringCanvas(16, (*this->display).getScreenHeight() - versionWidth - 8, version, 255, 255, 255, 8, 90);

  // Draw the icon
  IconImage IconImage;

  // Allocate the image
  uint8_t *image = (uint8_t *)FloppyHardware::malloc(IconImage::image_width * IconImage::image_height * 3 * sizeof(uint8_t));

  // Copy the image
  memcpy(image, IconImage.image_data, IconImage::image_width * IconImage::image_height * 3);

  // Draw the image
  this->drawImage(((*this->display).getScreenWidth() / 2) - IconImage::image_height + 2, ((*this->display).getScreenHeight() / 2) - (nameWidth / 2) - IconImage::image_height, IconImage::image_width, IconImage::image_height, image, 2, 0);

  // Free the memory at image_data
  FloppyHardware::free(image);

  // Draw the canvas
  (*this->display).drawCanvas();
}