##
# @file opencv_show_image.py
# @brief through 'opencv' show the image
# @author freedomDR, 1640145602@qq.com
# @version 1.0
# @date 2018-04-17

# coding=utf-8

import sys
import cv2 as cv

def show(image_files):
    images = []
    for v in image_files:
        image = cv.imread(v)
        images.append(image)
    while True:
        for v in range(len(image_files)):
            cv.imshow(image_files[v], images[v])
        k = cv.waitKey(500) & 0xFF
        if k == 27:
            break
    cv.destroyAllWindows()

if __name__ == '__main__':
    image_files = sys.argv[1:]
    if image_files != []:
        print(image_files)
        show(image_files)
    else:
        print('not found images')
    
