from PIL import Image, ImageDraw, ImageFont, ImageOps
import cv2
from math import floor

SIZE = 4096

NUM_COLUMNS, NUM_ROWS = 16, 8

CHAR_WIDTH, CHAR_HEIGHT = SIZE // NUM_COLUMNS, SIZE // NUM_ROWS
EM_SIZE = floor(CHAR_HEIGHT * 0.8)


atlas = Image.new("RGB", (SIZE, SIZE), (0, 0, 0, 255))
DRAW = ImageDraw.Draw(atlas)
FONT = ImageFont.truetype("consola.ttf", EM_SIZE)

_, _, _, h = FONT.getbbox("[")
HEIGHT_OFFSET = (CHAR_HEIGHT - h) // 2


for char in range(32, 128):
    index = char - 32
    r, c = index // NUM_COLUMNS, index % NUM_COLUMNS
    # DRAW.rectangle([c * CHAR_WIDTH, r * CHAR_HEIGHT,
    #                (c+1) * CHAR_WIDTH, (r+1) * CHAR_HEIGHT], (255, 0, 0) if (r + c) % 2 == 0 else (0, 255, 0))
    _, _, width, height = FONT.getbbox(chr(char))
    dx, dy = CHAR_WIDTH - width, CHAR_HEIGHT - height
    DRAW.text((c * CHAR_WIDTH + dx // 2, r * CHAR_HEIGHT + HEIGHT_OFFSET),
              chr(char) if char < 128 else chr(0xFFFD), (255, 255, 255), FONT)

pixels = atlas.load()
for y in range(atlas.size[1]):
    for x in range(atlas.size[0]):
        if pixels[x, y] != (0, 0, 0) and pixels[x, y] != (255, 255, 255):
            pixels[x, y] = (255, 255, 255)

atlas.save("consolas4096.png")

# img = cv2.imread("input1.png")
# bw_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# _, threshold = cv2.threshold(bw_img, 0, 255, cv2.THRESH_BINARY)
# dist_transform = cv2.distanceTransform(
#     threshold, cv2.DIST_L1, cv2.DIST_MASK_3)
# cv2.imwrite("sdf1.png", dist_transform)

# # cv2.imshow("", bw_img)
# # cv2.waitKey(0)
# # cv2.imshow("", threshold)
# # cv2.waitKey(0)

# inv_img = cv2.bitwise_not(img)
# inv_bw_img = cv2.cvtColor(inv_img, cv2.COLOR_BGR2GRAY)
# _, inv_threshold = cv2.threshold(inv_bw_img, 240, 255, cv2.THRESH_BINARY)
# inv_dist_transform = cv2.distanceTransform(
#     inv_threshold, cv2.DIST_L1, cv2.DIST_MASK_3)
# cv2.imwrite("sdf2.png", inv_dist_transform)

# # cv2.imwrite("sdf.png", cv2.addWeighted(
# #     dist_transform, 1, inv_dist_transform, 1, 0))

# cv2.imshow("", inv_bw_img)
# cv2.waitKey(0)
# cv2.imshow("", inv_threshold)
# cv2.waitKey(0)
# cv2.imshow("", cv2.imread("sdf2.png"))
# cv2.waitKey(0)

# # cv2.destroyAllWindows()
