import sys
from glob import glob
from os.path import basename, dirname
from subprocess import run
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from nuke_ui import Ui_MainWindow

class NukeNoDriveDlg(QDialog):
    def __init__(self, *args, **kwargs):
        super(NukeNoDriveDlg, self).__init__(*args, **kwargs)

        self.setWindowTitle("Nuke")

        btn = QDialogButtonBox.Ok
        self.buttonBox = QDialogButtonBox(btn)

        self.buttonBox.accepted.connect(self.accept)
        self.msg = QLabel(self)
        self.msg.setText("Please select a drive!")

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.msg)
        self.layout.addWidget(self.buttonBox)
        self.setLayout(self.layout)

class MainW(QMainWindow, Ui_MainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setupUi(self)

        drvs = self.check_drvs()
        self.drive_list.addItems(drvs)

        self.drv = ""
        self.nreps = 1
        self.only_zero = False

        self.drive_list.currentIndexChanged.connect(self.update_drv)
        self.rep_spinbox.valueChanged.connect(self.update_nreps)
        self.rb_checkbox.stateChanged.connect(self.update_only_zero)
        self.go_button.clicked.connect(self.confirm)

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

    def update_nreps(self, i):
        self.nreps = i
        print("Reps: ", self.nreps)

    def update_only_zero(self, i):
        self.only_zero = bool(i)
        print("Only zero: ", self.only_zero)

    def confirm(self):
        if self.drv == "" or self.drv == "-- No Drive Selected --":
            dlg = NukeNoDriveDlg(self)
            dlg.exec_()
        else:
            cmd = "./nuke -n {} -Y ".format(self.nreps)
            if self.only_zero:
                cmd += "-z "
            cmd += self.drv
            print(cmd)

            msg = QMessageBox()

            msg.setText("All data on '{}' will be erased <b>PERMANENTLY!</b><br/>\
Do you <b>STILL</b> want to continue?".format(self.drv))

            msg.setWindowTitle("Nuke")
            msg.setStandardButtons(QMessageBox.Yes | QMessageBox.No)
            msg.buttonClicked.connect(self.nuke)
            msg.exec_()

    def nuke(self, resp):
        if resp.text() == "&Yes":
            print(self.drv + " going down now!")

            msg = QMessageBox()
            msg.setText("This may take a few minutes...")
            l = msg.layout()

            l.itemAtPosition(l.rowCount() - 1, 0).widget().hide()
            
            progress = QProgressBar()
            l.addWidget(progress, l.rowCount(), 0, 1, l.columnCount(), Qt.AlignCenter)
            msg.exec_()
            
        else:
            print("Abort.")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    myapp = MainW()
    myapp.show()
    sys.exit(app.exec_())
