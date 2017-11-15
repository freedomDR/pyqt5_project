#!bin/env python3
# coding: utf-8

import cv2
import numpy as np
import sys
sys.path.append('/mnt/sdb/python3_code/pyqt_opencv/test/')
from function_collection import my_utility

# 图像反转 function_index: 1
def reversal(img):
    res_img = [255-v for v in img]
    res_img = np.array(res_img)
    res_img.reshape(img.shape)
    return res_img

# 灰度直方图 function_index 2
def histogram(img,flag):
    x, y = np.histogram(img.ravel(),bins=np.array(0,256),density=flag)
    return x,y

# 直方图均衡化 function_index 3
def grayscale_histogram(img):
    res_image = cv2.equalizeHist(img)
    return res_image

# 分段线性变换 function_index 5
def linear_translation(img):
    middle = list(img.shape)
    r1 = np.min(img)
    s1 = 0.0
    r2 = np.max(img)
    s2 = 255.0
    k = (s2 - s1) // (r2 - r1)
    res_img = [[k * img[x][y] for y in range(middle[1])] for x in range(middle[0])]
    return np.array(res_img)

# 幂律变换 function_index 6
def gamma_translation(img, c, r):
    middle = list(img.shape)
    res_img = [[c*np.power(img[x][y],r)for y in range(middle[1])]for x in range(middle[0])]
    return  np.array(res_img)

# 平滑滤波器 function_index 7
def smooth_fliter(img):
    res_img = cv2.boxFilter(img,ddepth=-1,ksize=(5,5))
    return res_img

# 中值滤波器 function_index 8
def median_fliter(img):
    res_img = cv2.medianBlur(img,ksize=3)
    return res_img

# 旋转变换 function_index 9
def rotate_translation(img, degree):
    M = cv2.getRotationMatrix2D((img.shape[0]//2, img.shape[1]//2),degree,1)
    res_img = cv2.warpAffine(img, M,(img.shape[1],img.shape[0]))
    return res_img

# 水平，垂直变换 function_index 10
def vh_translation(img):
    M1 = np.float32([[1,0,50],[0,1,0]])
    res_img_h = cv2.warpAffine(img, M1, (img.shape[1], img.shape[0]))
    M2 = np.float32([[1, 0, 0], [0, 1, 50]])
    res_img_v = cv2.warpAffine(img, M2, (img.shape[1], img.shape[0]))
    return res_img_h, res_img_v

if __name__ == '__main__':
    pass