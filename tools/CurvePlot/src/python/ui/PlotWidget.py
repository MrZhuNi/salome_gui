from pyqtside import QtWidgets
from pyqtside.uic import loadUiGen
from .utils import completeResPath

class PlotWidget(QtWidgets.QMainWindow):
  def __init__(self):
    QtWidgets.QMainWindow.__init__(self)
    loadUiGen(completeResPath("PlotWidget.ui"), self)

  def clearAll(self):
    """ In test context, the PlotWidget is never fully deleted (because the PyQt binding
    of QTabWidget doesn't remove completly the references it holds).
    So clean up manually. 
    """
    self.toolBar = None
    self.setCentralWidget(None)
