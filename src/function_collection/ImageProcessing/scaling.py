# coding: utf-8
import cv2 as cv
import matplotlib.pyplot as plt
import sys, os
from PyQt5.QtWidgets import (QWidget, QGroupBox, QGridLayout, QVBoxLayout,QSlider,  QComboBox, QPushButton, QApplication, QHBoxLayout, QLabel, QLineEdit)
from PyQt5.QtCore import (QState, QStateMachine, pyqtSignal, Qt)
sys.path.append('/mnt/sdb/python3_code/pyqt_opencv/src/')
from function_collection import my_utility

class Me(QWidget):

    def __init__(self, path):
        super().__init__()
        self.kinds = ['INTER_NEAREST', 'INTER_LINEAR', 'INTER_CUBIC', 'INTER_AREA', 'INTER_LANCZOS4', 'INTER_LINEAR_EXACT', 'INTER_MAX', 'WARP_FILL_OUTLIERS', 'WARP_INVERSE_MAP']
        self.kinds_value = []
        self.path = path
        self.h_x = 1
        self.v_y = 1
        self.kind = 0
        self.my_ui()

    def my_ui(self):
        v_box = QVBoxLayout()
        config_box = QGroupBox('配置')
        grid = QGridLayout()
        label_one = QLabel('当前状态')
        self.choose_button = QPushButton()
        self.machine = QStateMachine()
        zoom_state = QState(self.machine)
        shrink_state = QState(self.machine)
        self.machine.setInitialState(zoom_state)
        zoom_state.assignProperty(self.choose_button, 'text', '放大')
        shrink_state.assignProperty(self.choose_button, 'text', '缩小')
        shrink_state.addTransition(self.choose_button.clicked, zoom_state)
        zoom_state.addTransition(self.choose_button.clicked, shrink_state)
        self.machine.start()
        grid.addWidget(label_one, 0, 0)
        grid.addWidget(self.choose_button, 0 ,1)

        grid.addWidget(QLabel('倍数'), 1, 0)
        slider = QSlider(Qt.Horizontal)
        slider.setMaximum(5)
        slider.setMinimum(1)
        slider.setTickInterval(1)
        slider.setTickPosition(QSlider.TicksBothSides)
        slider.setSingleStep(1)
        slider.valueChanged.connect(self.value_deal)
        grid.addWidget(slider, 1, 1)

        kind = QComboBox()
        for v in self.kinds: kind.addItem(v)
        kind.activated.connect(self.choosed)
        grid.addWidget(QLabel('类别：'), 2, 0)
        grid.addWidget(kind, 2, 1)

        button_show = QPushButton('显示')
        button_show.clicked.connect(self.show_image)

        config_box.setLayout(grid)
        v_box.addWidget(config_box)
        v_box.addWidget(button_show)
        self.setLayout(v_box)
        self.resize(10, 300)
        self.show()

    def choosed(self, x):
        self.kind = x
        pass

    def value_deal(self, x):
        self.h_x = x
        self.v_y = x
        print(self.choose_button.text())
        text = self.choose_button.text()
        pass

    def show_image(self):
        if text == '放大':
            pass
        else:
            self.h_x = 1/self.h_x
            self.v_y = 1/self.v_y
        origin_img = cv.imread(self.path, cv.IMREAD_COLOR)
        res_img = cv.resize(origin_img, None, fx = self.h_x, fy = self.v_y, interpolation = self.kind)
        # my_utility.custom_show(origin_img, [1,2,1])
        # my_utility.custom_show(res_img, [1,2,2])
        # my_utility.plt.show()
        names = ['origin_image.png', 'res_image.png']
        images = [origin_img, res_img]
        my_utility.custom_show_opencv(names, images)
        print('origin shape', origin_img.shape)
        print('res shape', res_img.shape)
        pass


def deal(path):
    me = Me()
    #origin_img = cv.imread(path, cv.IMREAD_COLOR)
    #res_img = origin_img.copy()
    #print('-----------------')
    ##while True:
    ##    cv.imshow('origin', origin_img)
    ##    #cv.imshow('deal', res_img)
    ##    k = cv.waitKey(0)&0xFF
    ##    if k == 27:
    ##        break
    ##    #res_img = cv.resize(origin_img, None, fx = me.h_x, fy = me.v_y, interpolation = me.kind)
    ##    res_img = cv.resize(origin_img, None, fx = 2, fy = 2, interpolation = cv.INTER_AREA)
    #cv.imshow('origin', origin_img)
    #cv.imshow('deal', res_img)
    #cv.waitKey(5000)
    #cv.destroyAllWindows()


if __name__ == '__main__':
    path = '/home/dr/图片/董睿一寸照.png'
    # if os.path.isfile(path):
    #     img = cv.imread(path, cv.IMREAD_COLOR)
    #     plt.imshow(img)
    #     plt.show()
    # else:
    #     print('file is not exit')
    app = QApplication(sys.argv)
    me = Me()
    # deal(path)
    # self.choose_button = QPushButton()
    # machine = QStateMachine()
    # zoom_state = QState(machine)
    # shrink_state = QState(machine)
    # machine.setInitialState(zoom_state)
    # zoom_state.assignProperty(self.choose_button, 'text', '放大')
    # shrink_state.assignProperty(self.choose_button, 'text', '缩小')
    # shrink_state.addTransition(self.choose_button.clicked, zoom_state)
    # zoom_state.addTransition(self.choose_button.clicked, shrink_state)
    # machine.start()
    # self.choose_button.show()
    sys.exit(app.exec_())

