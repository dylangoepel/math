import PIL.Image
import regex
import numpy as np
import sys

# encodes a uint8[y][x] matrix into an image

if len(sys.argv) != 2:
    sys.exit(1)

header = sys.stdin.readline().removesuffix("\n")
m = regex.match("([0-9]+),([0-9]+)", header)
(width, height) = (int(m.group(1)), int(m.group(2)))

data = sys.stdin.buffer.read()
pixels = np.zeros((width, height, 3), dtype='uint8')

for x in range(width):
    for y in range(height):
        pixels[x, y, 0] = data[x + width * y]
        pixels[x, y, 1] = data[x + width * y]
        pixels[x, y, 2] = data[x + width * y]

img = PIL.Image.fromarray(pixels)
img.save(sys.argv[1])
