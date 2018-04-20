# coding=utf-8

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
import sys
import numpy as np
import cv2 as cv
sys.path.append('/mnt/sdb/python3_code/pyqt_opencv/src')
from function_collection import my_utility

class Me(QWidget):

    def __init__(self, path):
        super().__init__()
        self.my_ui()
        self.image_path = path
    
    def my_ui(self):
        v_box = QVBoxLayout()
        config_box = QGroupBox('配置')
        grid = QGridLayout()
        text1 = QLabel('仿射前')
        text1.setAlignment(Qt.AlignHCenter)
        grid.addWidget(text1, 0, 0, 1, 3)
        self.edit_before_1x = QLineEdit('50')
        self.edit_before_2x = QLineEdit('200')
        self.edit_before_3x = QLineEdit('50')
        self.edit_before_1y = QLineEdit('50')
        self.edit_before_2y = QLineEdit('50')
        self.edit_before_3y = QLineEdit('200')
        grid.addWidget(QLabel('第一个点'), 1, 0)
        grid.addWidget(self.edit_before_1x, 1, 1)
        grid.addWidget(self.edit_before_1y, 1, 2)
        grid.addWidget(QLabel('第二个点'), 2, 0)
        grid.addWidget(self.edit_before_2x, 2, 1)
        grid.addWidget(self.edit_before_2y, 2, 2)
        grid.addWidget(QLabel('第三个点'), 3, 0)
        grid.addWidget(self.edit_before_3x, 3, 1)
        grid.addWidget(self.edit_before_3y, 3, 2)
        text2 = QLabel('仿射后')
        text2.setAlignment(Qt.AlignHCenter)
        grid.addWidget(text2, 4, 0, 1, 3)
        self.edit_end_1x = QLineEdit('10')
        self.edit_end_2x = QLineEdit('200')
        self.edit_end_3x = QLineEdit('100')
        self.edit_end_1y = QLineEdit('100')
        self.edit_end_2y = QLineEdit('50')
        self.edit_end_3y = QLineEdit('250')
        grid.addWidget(QLabel('第一个点'), 5, 0)
        grid.addWidget(self.edit_end_1x, 5, 1)
        grid.addWidget(self.edit_end_1y, 5, 2)
        grid.addWidget(QLabel('第二个点'), 6, 0)
        grid.addWidget(self.edit_end_2x, 6, 1)
        grid.addWidget(self.edit_end_2y, 6, 2)
        grid.addWidget(QLabel('第三个点'), 7, 0)
        grid.addWidget(self.edit_end_3x, 7, 1)
        grid.addWidget(self.edit_end_3y, 7, 2)
        config_box.setLayout(grid)
        v_box.addWidget(config_box)
        run_button = QPushButton('运行')
        run_button.clicked.connect(self.deal)
        v_box.addWidget(run_button)
        self.setLayout(v_box)
        self.show()

    def deal(self):
        print('start deal:')
        origin_image = cv.imread(self.image_path, cv.IMREAD_COLOR)
        origin_image = origin_image[..., ::-1]
        before_1x = int(self.edit_before_1x.text())
        before_2x = int(self.edit_before_2x.text())
        before_3x = int(self.edit_before_3x.text())
        before_1y = int(self.edit_before_1y.text())
        before_2y = int(self.edit_before_2y.text())
        before_3y = int(self.edit_before_3y.text())
        end_1x = int(self.edit_end_1x.text())
        end_2x = int(self.edit_end_2x.text())
        end_3x = int(self.edit_end_3x.text())
        end_1y = int(self.edit_end_1y.text())
        end_2y = int(self.edit_end_2y.text())
        end_3y = int(self.edit_end_3y.text())
        pst1 = np.float32([[before_1x, before_1y],[before_2x, before_2y],[before_3x, before_3y]])
        pst2 = np.float32([[end_1x, end_1y],[end_2x, end_2y],[end_3x, end_3y]])
        M = cv.getAffineTransform(pst1, pst2)
        res_image = cv.warpAffine(origin_image, M, (origin_image.shape[1], origin_image.shape[0]))
        my_utility.custom_show(origin_image, [1,2,1])
        my_utility.custom_show(res_image, [1,2,2])
        my_utility.plt.show()
        pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    me = Me('/home/dr/图片/董睿一寸照.png')
    sys.exit(app.exec_())
    pass
