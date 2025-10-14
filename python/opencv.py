import cv2
import tkinter as tk
from tkinter import filedialog
import os

def chapter2(src, dst_union) :
    h, w, d = src.shape
    for y in range(h):
        for x in range(w):
            pix_alue  = src[y,x]
            # dst_union[y,x] = max(pix_alue, 100)
    return dst_union
            

def average(img, x, y, boxradius):
    avg = 0
    for dy in range(-boxradius, boxradius+1):
        for dx in range(-boxradius, boxradius+1):
            avg += img[y + dy, x + dx]
    return avg / 9 * 3


def getImage() -> any:
    root = tk.Tk()
    root.withdraw()
    start = os.path.abspath("~")

    file_path = filedialog.askopenfilename(
        title="Select a File", filetypes=[("Images", "*.png *.jpg *.jpeg *.ppm *.jxl")]
    )
    
    print("Selected file:", file_path)
    return cv2.imread(file_path)


img_grayscale = getImage()


h, w, d = img_grayscale.shape 


boxradius = 1

for y in range(boxradius, h - boxradius):
    for x in range(boxradius, w - boxradius):
        img_grayscale[y, x] = average(img_grayscale, x, y, boxradius) # pyright: ignore[reportOptionalSubscript]


# cv2.imshow("Grayscale", d)

# cv2.resizeWindow("Grayscale", 200, 200)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
cv2.imwrite("out.jpg", img_grayscale)

# picture1 = getImage()

# picture2 = getImage()

# cv2.imshow("dst_union", chapter2(picture1, picture2))

