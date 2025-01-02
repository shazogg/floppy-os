# generate_image_hpp.py

import numpy as np
from PIL import Image

def generate_image_hpp(image_path, output_file):
    # Open the image
    img = Image.open(image_path)

    # Convert image to RGB mode
    img = img.convert('RGB')
    
    # Convert the image to Array
    img_array = np.array(img)
    
    # Get the dimensions of the image
    height, width, _ = img_array.shape
    
    # Convert the image array to a byte array
    byte_array = img_array.tobytes()
    
    # Write the byte array to a .hpp file
    with open(output_file, 'w') as f:
        # Once and imports
        f.write("// Generated  image file\n")
        f.write("#pragma once\n\n")
        
        f.write("// Includes\n")
        f.write("#include <cstdint>\n\n")

        # Class definition
        f.write("// Image class\n")
        f.write("class ImageClass\n{\npublic:")

        # Get image width and height
        f.write("    static const uint16_t image_width = {};\n".format(width))
        f.write("    static const uint16_t image_height = {};\n".format(height))
        
        f.write("const uint8_t image_data[" + str(width) + " * " + str(height) +" * 3] = {\n")
        
        # Écrire les bytes de l'image
        for i, byte in enumerate(byte_array):
            f.write("    {},".format(byte))
            if (i + 1) % (width * 3) == 0:
                f.write("\n")
        
        f.write("\n};\n")
      
      # End of the class
        f.write("};\n")
        
        # Close the file
        f.close()

# Path to the image
image_path = "mc.png"

# Output file
output_file = "image_data.hpp"

# Generate the image.hpp file
generate_image_hpp(image_path, output_file)