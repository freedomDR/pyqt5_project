#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import cv2
import numpy as np
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from function_collection import custom_deal, my_utility
import function_collection.ImageProcessing.scaling as scaling
import function_collection.ImageProcessing.AffineTransformation as AffineTransformation

class Me(QWidget):
    function_map_index = {1: '灰度图像反转', 2: '灰度直方图', 3: '直方图均衡化',
                          4: '直方图规定化', 5: '分段线性变换', 6: '幂律变换',
                          7: '平滑滤波器', 8: '中值滤波器', 9: '旋转变换',
                          10: '水平垂直平移变换', 11: '图像金字塔', 12: '图像三种插值改变图像大小',
                          13: '放射变换'}
    function_map_key = dict(zip(list(function_map_index.values()), list(function_map_index.keys())))
    
    function_widgets = [None]*14
    
    file_addr = ''

    def __init__(self):
        super().__init__()
        self.my_ui()
        self.choose_id = 0
        default_path = '/home/dr/图片/董睿一寸照.png'
        self.anotherWidget = None
        self.file_addr = None
        self.q_text_editor = None

    def my_ui(self):
        v_box = QVBoxLayout()

        grid = QGridLayout()

        q_button = QPushButton('选择文件', self)
        q_button.clicked.connect(self.show_dialog)
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
        v_box.addWidget(q_run_button, alignment=Qt.AlignHCenter)
        v_box.addStretch(2)
        self.setLayout(v_box)

        self.resize(500, 400)
        self.center()
        self.setWindowTitle('大项目')
        self.setWindowIcon(QIcon('icon.png'))
        self.show()

    # 点击退出按钮触发函数
    def closeEvent(self, q_close_event):
        reply = QMessageBox.question(QMessageBox(self), 'message', '你确定要退出吗', QMessageBox.No | QMessageBox.Yes,
                                     QMessageBox.Yes)

        if reply == QMessageBox.Yes:
            q_close_event.accept()
        else:
            q_close_event.ignore()

    # 移动应用位置函数
    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
        pass

    # 选择文件处理函数
    def show_dialog(self):
        file_name = QFileDialog.getOpenFileName(QFileDialog(self), 'open file', '/mnt/sdb/opencv/ImageMaterial')
        self.file_addr_text.setText(str(file_name[0]))
        self.file_addr = file_name[0]
        print('file_addr: ', file_name[0])

    # 功能列表选择触发函数
    def on_activated(self, text):
        print(text, self.function_map_key[text])
        self.choose_id = self.function_map_key[text]

    # 演示按钮绑定函数
    def run_function(self):
        self.deal(self.file_addr, self.choose_id)

    # 功能处理函数
    def deal(self, file_addr, function_index):
        # print('-----',function_map_index[function_index],'-----')
        origin_image = cv2.imread(file_addr, cv2.IMREAD_COLOR)
        if function_index == 1:
            res_image = custom_deal.reversal(origin_image)
            my_utility.custom_show(origin_image, [1, 2, 1])
            my_utility.plt.title('origin image')
            my_utility.custom_show(res_image, [1, 2, 2])
            my_utility.plt.title('reversal image')
        elif function_index == 2:
            my_utility.custom_show(origin_image, [1, 2, 1])
            res_image = cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY)
            my_utility.custom_show_hist(res_image, [1, 2, 2])
        elif function_index == 3:
            my_utility.custom_show(origin_image, [1, 2, 1])
            res_image = cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY)
            res_image = custom_deal.grayscale_histogram(res_image)
            my_utility.custom_show(res_image, [1, 2, 2])
        elif function_index == 4:
            self.q_text_editor.setPlainText('还未实现')
            return
        elif function_index == 5:
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
        elif function_index == 6:
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
        elif function_index == 7:
            my_utility.custom_show(origin_image, [1, 2, 1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.smooth_fliter(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY))
            my_utility.custom_show(res_image, [1, 2, 2])
            my_utility.plt.title('3x3 smooth filter')
        elif function_index == 8:
            my_utility.custom_show(origin_image, [1, 2, 1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.median_fliter(cv2.cvtColor(origin_image, code=cv2.COLOR_RGB2GRAY))
            my_utility.custom_show(res_image, [1, 2, 2])
            my_utility.plt.title('3x3 median filter')
        elif function_index == 9:
            my_utility.custom_show(origin_image, [1, 2, 1])
            my_utility.plt.title('origin image')
            res_image = custom_deal.rotate_translation(origin_image, 90)
            my_utility.custom_show(res_image, [1, 2, 2])
            my_utility.plt.title('60 degree')
        elif function_index == 10:
            my_utility.custom_show(origin_image, [1, 3, 1])
            my_utility.plt.title('origin image')
            res_image_h, res_image_v = custom_deal.vh_translation(origin_image)
            my_utility.custom_show(res_image_h, [1, 3, 2])
            my_utility.plt.title('horizontal')
            my_utility.custom_show(res_image_v, [1, 3, 3])
            my_utility.plt.title('vertical')
        elif function_index == 11:
            my_utility.custom_show(origin_image, [1, 3, 1])
            my_utility.plt.title('origin image')
            res_down_image, res_up_image = custom_deal.pyramid(origin_image)
            my_utility.custom_show(res_down_image, [1, 3, 2])
            my_utility.custom_show(res_up_image, [1, 3, 3])
            pass
        elif function_index == 12:
            self.function_widgets[function_index] = scaling.Me(file_addr)
            pass
        elif function_index == 13:
            self.function_widgets[function_index] = AffineTransformation.Me(file_addr)
            pass
        if function_index != 12:
            my_utility.plt.show()

    def nothing(self, x):
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = Me()
    sys.exit(app.exec_())
