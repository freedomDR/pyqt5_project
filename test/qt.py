#!/bin/env python3
# -*- coding: utf-8 -*-
import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QMessageBox, QDesktopWidget, QLineEdit, QFileDialog, \
    QComboBox, QGridLayout, QHBoxLayout, QVBoxLayout, QLabel
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QCoreApplication


class Me(QWidget):
    def __init__(self):
        super().__init__()
        self.my_ui()

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
        choose_button.addItem('直方图均衡化')
        choose_button.addItem('直方图规定化')
        choose_button.addItem('')
        choose_button.move(0, 50)
        choose_button.setCurrentIndex(1)
        choose_button.activated[str].connect(self.on_activated)

        grid.addWidget(q_button, 1, 0)
        grid.addWidget(self.file_addr_text, 1, 1)
        grid.addWidget(q_func_button, 2, 0)
        grid.addWidget(choose_button, 2, 1)

        q_run_button = QPushButton('演示')
        q_run_button.clicked.connect(self.run_function)

        v_box.addLayout(grid)
        v_box.addWidget(q_run_button)
        v_box.addStretch(3)
        self.setLayout(v_box)

        self.resize(500, 400)
        self.center()
        self.setWindowTitle('大项目')
        self.setWindowIcon(QIcon('icon.png'))
        self.show()

    def closeEvent(self, QCloseEvent):
        reply = QMessageBox.question(self, 'message', '你确定要退出吗', QMessageBox.No | QMessageBox.Yes, QMessageBox.Yes)

        if reply == QMessageBox.Yes:
            QCloseEvent.accept()
        else:
            QCloseEvent.ignore()

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
        pass

    def showDialog(self):
        fname = QFileDialog.getOpenFileName(self, 'open file', '/mnt/sdb')
        self.file_addr_text.setText(str(fname[0]))
        print(fname[0])

    def test(self):
        print('test')

    def on_activated(self, text):
        print(text)

    def run_function(self):
        print(dir(self))
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = Me()
    sys.exit(app.exec_())
