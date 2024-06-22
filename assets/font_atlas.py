# from PIL import Image, ImageDraw, ImageFont
# SIZE = 4096

# NUM_COLUMNS, NUM_ROWS = 16, 8

# ANTI_ALIASED = True

# CHAR_WIDTH, CHAR_HEIGHT = SIZE // NUM_COLUMNS, SIZE // NUM_ROWS
# EM_SIZE = CHAR_HEIGHT


# atlas = Image.new("RGBA", (SIZE, SIZE), (0, 0, 0, 0))
# DRAW = ImageDraw.Draw(atlas)
# # FONT = ImageFont.truetype("C:\Windows\Fonts\consola.ttf", 16)
# FONT = ImageFont.truetype("consola.ttf", EM_SIZE)
# # print(FONT.getbbox("ABcM"))


# for char in range(32, 127):
#     index = char - 32
#     r, c = index // NUM_COLUMNS, index % NUM_COLUMNS
#     DRAW.text((c * CHAR_WIDTH, r * CHAR_HEIGHT),
#               chr(char), (255, 255, 255), FONT)

# if not ANTI_ALIASED:
#     pixels = atlas.load()
#     for y in range(atlas.size[1]):
#         for x in range(atlas.size[0]):
#             if pixels[x, y][3] != 0 and pixels[x, y][3] < 255:
#                 pixels[x, y] = (0, 0, 0, 0)


# atlas.save("new.png")

print(chr(0xFFFD))
