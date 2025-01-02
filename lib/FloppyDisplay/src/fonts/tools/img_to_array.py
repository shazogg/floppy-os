# Imports
from PIL import Image

# Constants
IMAGE_PATH = 'font.png'  # Path to the image file
OUTPUT_FILE_PATH = 'font.h'  # Path to the output file
VALID_CHARACTERS = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~" # Characters included in the font

# Function to get the Unicode name of a character
def get_char_name(char):
    unicode_code = ord(char)
    return "U_{:X}".format(unicode_code)


# Load the image
image = Image.open(IMAGE_PATH)

# Convert the image to grayscale
image = image.convert("L")

# Get the dimensions of the image
width, height = image.size

# Check if the image is composed of 8x8 pixel characters
if width % 8 != 0 or height % 8 != 0:
    print("The image is not composed of 8x8 pixel characters. Please provide an image with 8x8 pixel characters.")
    exit()

# List to store the pixel values of each character
char_pixel_values = []

# Process each character in the image
current_char = 0
for char_y in range(0, height, 8):
    for char_x in range(0, width, 8):
        # If we have processed all the characters, we can exit the loop
        if current_char >= len(VALID_CHARACTERS):
            break

        # Process the character
        char_lines = []
        # For each line of the character
        for line in range(8):
            line_value = ""
            # For each pixel of the line
            for pixel in range(8):
                pixel = image.getpixel((char_x + pixel, char_y + line))
                if pixel < 128:
                    line_value += "1"
                else:
                    line_value += "0"

            # convert the line value to an integer
            line_value = int(line_value, 2)

            # Add the value of the line the the character's list of values
            char_lines.append(line_value)

        # Add value of the char to the list
        char_pixel_values.append(char_lines)
        current_char += 1

# Write the character pixel values to the output file
with open(OUTPUT_FILE_PATH, "w") as f:
    f.write("const uint8_t chars[{}][8] = {{\n".format(len(char_pixel_values)))
    for i, pixel_value in enumerate(char_pixel_values):
        f.write("    {")
        for j, value in enumerate(pixel_value):
            # If it's the last value of the line, we don't want a comma at the end
            if j == len(pixel_value) - 1:
                f.write("0x{:02X}".format(value))
            else:
                f.write("0x{:02X}, ".format(value))

        # If it's the last character, we don't want a comma at the end
        if i == len(char_pixel_values) - 1:
            f.write("}\n")
        else:
            f.write("},\n")

    f.write("};\n")
