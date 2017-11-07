#!bin/env python3.6
#coding: utf-7

from matplotlib import pyplot as plt

def custom_show(img, position):
    plt.subplot(position[0],position[1],position[2])
    plt.imshow(img, cmap='Greys_r')
    plt.xticks([]), plt.yticks([])
def custom_show_hist(img,position):
    plt.subplot(position[0], position[1], position[2])
    plt.hist(img.ravel(), bins=[i for i in range(1,256)], density=True)