from pyqtside import QtWidgets, QtCore
from pyqtside.uic import loadUiGen
from .utils import completeResPath

class CurveTreeDockWidget(QtWidgets.QDockWidget):
  def __init__(self):
    QtWidgets.QDockWidget.__init__(self)
    loadUiGen(completeResPath("CurveTreeDockWidget.ui"), self)
    self.treeWidget.setHeaderLabel ("Plots")
    self.treeWidget.sortByColumn(0, QtCore.Qt.AscendingOrder)
    self.treeWidget.setSortingEnabled(True);
    self.treeWidget.setColumnHidden(1, True);

  def getTreeWidget(self):
      """
      :returns: QTreeWidget -- the (curve) browser
      """
      return self.treeWidget
