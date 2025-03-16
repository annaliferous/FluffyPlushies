from PIL import Image

# Load the image
img = Image.open("pixel_heart.png").convert("L")  # Convert to grayscale for 8-bit data

# Ensure the image is 8-bit grayscale
img = img.convert("P", palette=Image.ADAPTIVE, colors=256)

# Get the pixel data
pixels = list(img.getdata())

# Image dimensions
width, height = img.size

# Open the output .h file
with open("image.h", "w") as f:
    # Write the header guard
    f.write("#ifndef IMAGE_H\n")
    f.write("#define IMAGE_H\n\n")

    # Write the image width, height, and pixel array declaration
    f.write(f"#define IMAGE_WIDTH {width}\n")
    f.write(f"#define IMAGE_HEIGHT {height}\n\n")
    f.write("const unsigned char image_data[] = {\n")

    # Write the pixel data as hex values in rows
    for i, pixel in enumerate(pixels):
        f.write(f"0x{pixel:02X}, ")  # Format pixel as 2-digit hex
        if (i + 1) % width == 0:
            f.write("\n")

    f.write("};\n\n")
    f.write("#endif // IMAGE_H\n")
