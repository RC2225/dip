# -*- coding: utf-8 -*-
"""
Created on Tue Sep 16 09:17:34 2025
 
@author: ngoctuyenle
"""# The function cv2.imread() is used to read an image.

 
# import the cv2 library
import cv2
# The function cv2.imread() is used to read an image.
#src = cv2.imread('Y:/Lee/Databases/UPS_RF/FLIR0013.jpg') #Desktop "C:\Mac\Home\Pictures\download.jpg"
src = cv2.imread('C:\\Mac\\Home\\Pictures\\download.jpg')
# OpenCV stores color images in BGR format by default
blue_channel, green_channel, red_channel = cv2.split(src)
 
# Display the individual channels as grayscale images
cv2.imshow('Blue Channel', blue_channel)
cv2.imshow('Green Channel', green_channel)
cv2.imshow('Red Channel', red_channel)
 
 
merged_image = cv2.merge((green_channel, blue_channel, red_channel ))
 
# grayscale = (blue_channel+green_channel+red_channel)/3
# grayscale = cv2.normalize(grayscale, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX)
# The function cv2.imshow() is used to display an image in a window.
cv2.imshow('original image',src)
cv2.imshow('merged_image',merged_image)
 
blue_channel_negative = 255 - blue_channel
#cv2.imshow('grayscale',grayscale)  
 
#img1 = 255 -  img_grayscale;
cv2.imshow("blue_channel_imvert ", blue_channel_negative )
 
 
# Get the height and width of the image
height, width, channels = src.shape
 
# Loop through each row
for y in range(height):
    # Loop through each column in the current row
    for x in range(width):
        # Access the pixel value at (x, y)
        # For a color image (BGR), it will return a tuple (B, G, R)
        # For a grayscale image, it will return a single integer intensity
        pixel_value = src[y, x]
 
        # You can then process or print the pixel value
        # For example, print the BGR values for a color image:
        if channels == 3:
            b, g, r = pixel_value
            # print(f"Pixel at ({x}, {y}): B={b}, G={g}, R={r}")
        else: # Grayscale image
            # print(f"Pixel at ({x}, {y}): Intensity={pixel_value}")
            pass # Do something with the grayscale pixel value
 
 
# waitKey() waits for a key press to close the window and 0 specifies indefinite loop
cv2.waitKey(0)
# cv2.destroyAllWindows() simply destroys all the windows we created.
cv2.destroyAllWindows()
 
# The function cv2.imwrite() is used to write an image.
#cv2.imwrite('grayscale.jpg',img_grayscale)
 