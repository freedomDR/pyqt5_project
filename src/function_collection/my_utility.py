# !bin/env python3.6
# coding: utf-8

from matplotlib import pyplot as plt
import cv2 as cv
import os

SRC = '/mnt/sdb/python3_code/pyqt_opencv/'

def custom_show(img, position):
    plt.subplot(position[0], position[1], position[2])
    plt.imshow(img, cmap='Greys_r')
    plt.xticks([]), plt.yticks([])


def custom_show_hist(img, position):
    plt.subplot(position[0], position[1], position[2])
    plt.hist(img.ravel(), bins=[i for i in range(1, 256)], density=True)

def custom_show_opencv(names, images):
    cur_dir = SRC+'src/function_collection/'
    os.chdir(cur_dir)
    for name, image in zip(names, images):
        cv.imwrite(name, image)
    files = ' '.join(names)
    cmd = 'python3 opencv_show_image.py '+files
    print('cmd: ', cmd)
    os.system(cmd)

