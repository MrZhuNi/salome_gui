from pyqtside.QtWidgets import QDialog, QColorDialog, QMessageBox
from pyqtside.QtGui import QIcon, QPixmap, QColor
from pyqtside.QtCore import pyqtSlot
from pyqtside.uic import loadUiGen
from .utils import completeResPath, Logger

class PlotSettings(QDialog):
  def __init__(self):
    QDialog.__init__(self)
    loadUiGen(completeResPath("PlotSettings.ui"), self)
    self.initialize()

  def initialize(self):
    self.legendPositionComboBox.addItem("Bottom")
    self.legendPositionComboBox.addItem("Right")
    self._r = 0
    self._g = 0
    self._b = 1

  @pyqtSlot(int)
  def onShowLegend(self, index):
    if index > 0 :
      self.legendPositionComboBox.setEnabled(True)
    else :
      self.legendPositionComboBox.setEnabled(False)

  @pyqtSlot()
  def onChangeColor(self):
    col = QColorDialog.getColor()

    if col.isValid():
      r, g, b = [c/255.0 for c in col.getRgb()[:3]]
      self.setRGB(r, g, b)

  def setSelectedCurveName(self, name):
    self.nameCurve.setText(name)
    if name :
      Logger.Debug("show curve panel")
      self.selectedCurvePanel.setTitle("Selected curve : " + name)
      self.selectedCurvePanel.show()
    else :
      Logger.Debug("hide curve panel")
      self.selectedCurvePanel.hide()

  def setRGB(self, r, g, b):
    self._r = r
    self._g = g
    self._b = b
    self.colorCurve.setIcon(QIcon(self.drawColorPixmap(int(r*255), int(g*255), int(b*255))))

  def getRGB(self):
    return self._r, self._g, self._b

  def drawColorPixmap(self, r, g, b):
    pix = QPixmap( 16, 16 )
    color = QColor(r, g, b)
    pix.fill(color)
    return pix

  def accept(self):
    xminText = str(self.axisXMinEdit.text())
    xmaxText = str(self.axisXMaxEdit.text())
    yminText = str(self.axisYMinEdit.text())
    ymaxText = str(self.axisYMaxEdit.text())
    if (yminText == "" or ymaxText == "") :
      QMessageBox.critical(self, "Plot settings", "A field \"YMin\" or \"YMax\" is empty")
    else :
      try:
        xmin = float(xminText)
      except ValueError:
        QMessageBox.critical(self, "Plot settings", "It is not possible to convert XMin")
      try:
        xmax = float(xmaxText)
      except ValueError:
        QMessageBox.critical(self, "Plot settings", "It is not possible to convert XMax")
      try:
        ymin = float(yminText)
      except ValueError:
        QMessageBox.critical(self, "Plot settings", "It is not possible to convert YMin")
      try:
        ymax = float(ymaxText)
      except ValueError:
        QMessageBox.critical(self, "Plot settings", "It is not possible to convert YMax")
      if ((xmax-xmin) == 0) :
        QMessageBox.critical(self, "Plot settings", "XMax is is equal to XMin.")
        return
      if ((ymax-ymin) == 0) :
        QMessageBox.critical(self, "Plot settings", "YMax is is equal to YMin.")
        return
      if ((xmax-xmin) < 0) :
        QMessageBox.warning(self, "Plot settings", "XMax is less than XMin.")
      if ((ymax-ymin) < 0) :
        QMessageBox.warning(self, "Plot settings", "YMax is less than YMin.")
      super(PlotSettings, self).accept()
