from . import _use_pyqt
if _use_pyqt:
  from PyQt5.uic import loadUi as loadUiGen
else:
  from .pyside_dynamic import loadUi as loadUiGen


