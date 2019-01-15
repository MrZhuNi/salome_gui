"""
Group under one hat PySide and PyQt5. PyQt5 is tried first.
"""

try:
  import os
  if os.getenv("CURVEPLOT_FORCE_PYSIDE") is not None:
    raise Exception
  import PyQt5
  _use_pyqt = True
  print("Using PyQt5 run-time ...")
except:
  try:
    import PySide
    _use_pyqt = False
    print("Using PySide run-time ...")
  except:
    raise Exception("Neither PyQt5 nor PySide could be imported!")

# Matplotlib has to be handled very early, otherwise it will switch to whatever it
# finds first on the machine
try: 
  import matplotlib
  if _use_pyqt:  back = 'PyQt5'
  else:          back = 'PySide'
  # As advised by MatPlotlib:
  #   "The backend.qt5 rcParam was deprecated in version 2.2.  In order to force the use of a specific Qt binding, either import that binding first, or set the QT_API environment variable.
  #   mplDeprecation)"
  from matplotlib.backends import backend_qt5agg
  print("Matplotlib found - Set matplotlib backend to '%s'!" % back) 
except:
  # No matplotlib, silently discard err message.
  pass
