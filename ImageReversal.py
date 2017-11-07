import cv2
import numpy as np
from matplotlib import pyplot as plt


img = cv2.imread('../../ImageMaterial/DIP3/Fig0304(a)(breast_digital_Xray).tif',cv2.IMREAD_COLOR)
print(img)
middle = img.shape
plt.subplot(1,2,1)
plt.imshow(img)
plt.xticks([]), plt.yticks([])
plt.title('original image')

imgCopy = np.copy(img)
for i in range(middle[0]):
    for j in range(middle[1]):
        for k in range(middle[2]):
            imgCopy[i][j][k] = 255 - imgCopy[i][j][k]

plt.subplot(1,2,2)
plt.imshow(imgCopy)
plt.xticks([]), plt.yticks([])
plt.title('processed image')
plt.show()

#cv2.waitKey(3000)


