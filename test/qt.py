#!/bin/env python3
# -*- coding: utf-8 -*-
import sys
import cv2
import os
import numpy as np
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QMessageBox, QDesktopWidget, QLineEdit, QFileDialog, \
    QComboBox, QGridLayout, QHBoxLayout, QVBoxLayout, QLabel, QPlainTextEdit
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QCoreApplication

sys.path.append('/mnt/sdb/python3_code/pyqt_opencv/test')
from function_collection import custom_deal, my_utility


class Me(QWidget):
    function_map_index = {1: '灰度图像反转', \
                          2: '灰度直方图', \
                          3: '直方图均衡化', \
                          4: '直方图规定化', \
                          5: '分段线性变换', \
                          6: '幂律变换', \
                          7: '平滑滤波器', \
                          8: '中值滤波器', \
                          9: '旋转变换', \
                          10: '水平垂直平移变换'}
    function_map_key = dict(zip(list(function_map_index.values()), list(function_map_index.keys())))

    file_addr = ''

    def __init__(self):
        super().__init__()
        self.my_ui()
        self.choose_id = 0

    def my_ui(self):

        v_box = QVBoxLayout()

        grid = QGridLayout()

        q_button = QPushButton('选择文件', self)
        q_button.clicked.connect(self.showDialog)
        q_button.resize(q_button.sizeHint())
        q_button.move(0, 0)

        self.file_addr_text = QLineEdit()

        q_func_button = QPushButton('选择功能')
        q_func_button.setCheckable(False)

        choose_button = QComboBox(self)
        for i in range(1, len(self.function_map_index) + 1):
            choose_button.addItem(self.function_map_index[i])
        choose_button.move(0, 50)
        choose_button.setCurrentIndex(1)
        self.choose_id = 2
        choose_button.activated[str].connect(self.on_activated)

        grid.addWidget(q_button, 1, 0)
        grid.addWidget(self.file_addr_text, 1, 1)
        grid.addWidget(q_func_button, 2, 0)
        grid.addWidget(choose_button, 2, 1)

        q_run_button = QPushButton('演示')
        q_run_button.clicked.connect(self.run_function)

        self.q_text_editor = QPlainTextEdit('test')
        self.q_text_editor.adjustSize()

        v_box.addLayout(grid)
        v_box.addWidget(q_run_button)
        v_box.addWidget(self.q_text_editor)
        v_box.addStretch(2)
        self.setLayout(v_box)

        self.resize(500, 400)
        self.center()
        self.setWindowTitle('大项目')
        self.setWindowIcon(QIcon('icon.png'))
        self.show()

    # 点击退出按钮触发函数
    def closeEvent(self, QCloseEvent):
        reply = QMessageBox.question(self, 'message', '你确定要退出吗', QMessageBox.No | QMessageBox.Yes, QMessageBox.Yes)

        if reply == QMessageBox.Yes:
            QCloseEvent.accept()
        else:
            QCloseEvent.ignore()

    # 移动应用位置函数
    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
        pass

    # 选择文件处理函数
    def showDialog(self):
        fname = QFileDialog.getOpenFileName(self, 'open file', '/mnt/sdb')
        self.file_addr_text.setText(str(fname[0]))
        self.file_addr = fname[0]
        print(fname[0])

    def test(self):
        print('test')

    # 功能列表选择触发函数
    def on_activated(self, text):
        print(text, self.function_map_key[text])
        self.choose_id = self.function_map_key[text]

    # 演示按钮绑定函数
    def run_function(self):
        self.deal(self.file_addr, self.choose_id)

    # 功能处理函数
    def deal(self, file_addr, funtion_index):
        origin_image = cv2.imread(file_addr, cv2.IMREAD_COLOR)
        if (funtion_index == 1):
            res_image = custom_deal.reversal(origin_image)
            my_utility.custom_show(origin_image, [1, 2, 1])
            my_utility.plt.title('origin image')
            my_utility.custom_show(res_image, [1, 2, 2])
            my_utility.plt.title('reversal image')
        elif (funtion_index == 2):
            my_utility.custom_show(origin_image, [1, 2, 1])
            res_image = cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY)
            my_utility.custom_show_hist(res_image, [1, 2, 2])
        elif (funtion_index == 3):
            my_utility.custom_show(origin_image, [1, 2, 1])
            res_image = cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY)
            res_image = custom_deal.grayscale_histogram(res_image)
            my_utility.custom_show(res_image, [1, 2, 3])
        elif (funtion_index == 4):
            self.q_text_editor.setPlainText('还未实现')
            pass
        elif (funtion_index == 5):
            my_utility.plt.figure(1)
            my_utility.plt.plot([0, 10, 15, 25], [0, 5, 20, 25], 'ro-')
            my_utility.plt.figure(2)
            my_utility.custom_show(origin_image, [1, 3, 1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.linear_translation(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY))
            my_utility.custom_show(res_image, [1, 3, 2])
            my_utility.plt.title('Contrast stretch image')
            ret, res_image = cv2.threshold(res_image, np.mean(res_image), 255, type=cv2.THRESH_BINARY)
            my_utility.custom_show(res_image, [1, 3, 3])
            my_utility.plt.title('threshold image')
            pass
        elif (funtion_index == 6):
            my_utility.custom_show(origin_image, [1, 4, 1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.gamma_translation(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY), 1, 0.6)
            my_utility.custom_show(res_image, [1, 4, 2])
            my_utility.plt.title('gamma = 0.6')
            res_image = custom_deal.gamma_translation(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY), 1, 0.4)
            my_utility.custom_show(res_image, [1, 4, 3])
            my_utility.plt.title('gamma = 0.4')
            res_image = custom_deal.gamma_translation(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY), 1, 0.3)
            my_utility.custom_show(res_image, [1, 4, 4])
            my_utility.plt.title('gamma = 0.3')
        elif (funtion_index == 7):
            my_utility.custom_show(origin_image, [1,2,1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.smooth_fliter(cv2.cvtColor(origin_image,code=cv2.COLOR_RGB2GRAY))
            my_utility.custom_show(res_image, [1,2,2])
            my_utility.plt.title('3x3 smooth fliter')
        elif (funtion_index == 8):
            my_utility.custom_show(origin_image, [1,2,1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.median_fliter(cv2.cvtColor(origin_image,code=cv2.COLOR_RGB2GRAY))
            my_utility.custom_show(res_image, [1,2,2])
            my_utility.plt.title('3x3 median fliter')
        elif (funtion_index == 9):
            my_utility.custom_show(origin_image,[1,2,1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.rotate_translation(origin_image, 60)
            my_utility.custom_show(res_image,[1,2,2])
            my_utility.plt.title('60 degree')
        elif (funtion_index == 10):
            my_utility.custom_show(origin_image,[1,3,1])
            my_utility.plt.title('origin image')
            res_image_h, res_image_v = custom_deal.vh_translation(origin_image)
            my_utility.custom_show(res_image_h,[1,3,2])
            my_utility.plt.title('horizontal')
            my_utility.custom_show(res_image_v, [1,3,3])
            my_utility.plt.title('vertical')
        my_utility.plt.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)

    w = Me()
    sys.exit(app.exec_())
