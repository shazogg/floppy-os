# Imports
from PIL import ImageFont, ImageDraw, Image

# Constants
FONT_PATH = 'dogica.ttf' # Path to the font file
OUTPUT_IMAGE_PATH = 'font.png' # Output image path
VALID_CHARACTERS = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~" # Characters to include in the font
CHAR_SIZE = 8 # 8x8 pixels
CHARS_PER_ROW = 16 # Number of characters per row

# Generate a dictionary of character arrays
def generate_char_array(font_path, char_size):
    # Load the font
    font = ImageFont.truetype(font_path, char_size)
    char_array = {}

    # Generate the character arrays
    for char in VALID_CHARACTERS:
        # Create a blank image with white background
        img = Image.new('RGB', (char_size, char_size), color='white')
        draw = ImageDraw.Draw(img)
        # Render the character onto the image
        draw.text((0, 0), char, font=font, fill='black')

        # Convert the image to a binary array
        char_data = []
        for y in range(char_size):
            row = []
            for x in range(char_size):
                pixel = img.getpixel((x, y))

                # Assuming black text on white background
                if pixel == (0, 0, 0):
                    row.append(1)  # Black pixel
                else:
                    row.append(0)  # White pixel
            char_data.append(row)
        
        # Add the character array to the dictionary
        char_array[char] = char_data
    
    return char_array


# Generate a continuous image with all the characters
def generate_continuous_image(font_path, char_size, chars_per_row, output_path):
    char_array = generate_char_array(font_path, char_size)
    row_count = (len(char_array) // chars_per_row) + 1 if len(char_array) % chars_per_row != 0 else len(char_array) // chars_per_row
    image_height = row_count * char_size
    image_width = char_size * chars_per_row
    image = Image.new('RGB', (image_width, image_height), color='white')

    draw = ImageDraw.Draw(image)
    y_offset = 0

    # Draw each character onto the image
    for char_code, char in enumerate(char_array):
        x_offset = (char_code % chars_per_row) * char_size
        draw.text((x_offset, y_offset), char, font=ImageFont.truetype(font_path, char_size), fill='black')
        if (char_code + 1) % chars_per_row == 0:
            y_offset += char_size

    # Save the image
    image.save(output_path)

# Main function
if __name__ == '__main__':
    # Generate the continuous image
    generate_continuous_image(FONT_PATH, CHAR_SIZE, CHARS_PER_ROW, OUTPUT_IMAGE_PATH)
