import sys
from glob import glob
from os.path import basename, dirname
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from nuke_ui import Ui_MainWindow

class MainW(QMainWindow, Ui_MainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setupUi(self)

        drvs = self.check_drvs()
        self.drive_list.addItems(drvs)
        self.drive_list.currentIndexChanged.connect(self.update_drv)
        self.go_button.clicked.connect(self.nuke)

    def check_drvs(self):
        final_drvs = ["-- No Drive Selected --"]
        drive_glob = "/sys/block/*/device"

        for d in glob(drive_glob):
            dev_name = basename(dirname(d))

            # We want to ignore /dev/sda*
            if dev_name[2] != "a":
                final_drvs.append("/dev/" + basename(dirname(d)))

        return final_drvs

    def update_drv(self, i):
        self.drv = self.drive_list.currentText()
        print("Current Drive: ", self.drv)

    def nuke(self):
        print("Hello")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myapp = MainW()
    myapp.show()
    sys.exit(app.exec_())
