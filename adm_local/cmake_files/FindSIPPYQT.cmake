#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

FIND_PROGRAM(SIP_EXECUTABLE sip)

SET(SIP_ROOT ${SIP_EXECUTABLE})
IF(SIP_ROOT)
GET_FILENAME_COMPONENT(SIP_ROOT ${SIP_ROOT} PATH)
GET_FILENAME_COMPONENT(SIP_ROOT ${SIP_ROOT} PATH)
ENDIF(SIP_ROOT)

IF(WINDOWS)
  SET(SIP_INCLUDES -I${SIP_ROOT}/include)
ELSE(WINDOWS)
  SET(SIP_INCLUDES -I${SIP_ROOT}/include/python${PYTHON_VERSION})
ENDIF(WINDOWS)

IF(WINDOWS)
  FIND_PROGRAM(PYUIC_EXECUTABLE NAMES pyuic4 pyuic4.bat)
ELSE(WINDOWS)
  FIND_PROGRAM(PYUIC_EXECUTABLE NAMES pyuic4)
ENDIF(WINDOWS)

SET(PYUIC_ROOT ${PYUIC_EXECUTABLE})
IF(PYUIC_ROOT)
GET_FILENAME_COMPONENT(PYUIC_ROOT ${PYUIC_ROOT} PATH)
GET_FILENAME_COMPONENT(PYUIC_ROOT ${PYUIC_ROOT} PATH)
ENDIF(PYUIC_ROOT)

IF(WINDOWS)
  SET(PYQT_SIPS ${PYUIC_ROOT}/sip)
ELSE(WINDOWS)
  IF(PYUIC_ROOT STREQUAL /usr)
    SET(PYQT_SIPS ${PYUIC_ROOT}/share/sip/PyQt4)
  ELSE(PYUIC_ROOT STREQUAL /usr)
    SET(PYQT_SIPS ${PYUIC_ROOT}/share/sip)
  ENDIF(PYUIC_ROOT STREQUAL /usr)
ENDIF(WINDOWS)

SET(PYQT_INCLUDES -I${PYQT_SIPS})
SET(PYQT_INCLUDES ${PYQT_INCLUDES} -I${PYQT_SIPS}/QtCore -I${PYQT_SIPS}/QtGui)
SET(PYQT_INCLUDES ${PYQT_INCLUDES} -I${PYQT_SIPS}/QtXml -I${PYQT_SIPS}/QtOpenGL)
SET(PYQT_INCLUDES ${PYQT_INCLUDES} -I${PYQT_SIPS}/QtAssistant -I${PYQT_SIPS}/QtDesigner)
SET(PYQT_INCLUDES ${PYQT_INCLUDES} -I${PYQT_SIPS}/QtNetwork -I${PYQT_SIPS}/QtSql)
SET(PYQT_INCLUDES ${PYQT_INCLUDES} -I${PYQT_SIPS}/QtSvg -I${PYQT_SIPS}/QtTest)

SET(qt_flag ${QTVERSION})
STRING(REPLACE "." "_" qt_flag ${qt_flag})

IF(WINDOWS)
  SET(ws_flag WS_WIN)
ELSE(WINDOWS)
  SET(ws_flag WS_X11)
ENDIF(WINDOWS)

SET(PYQT_SIPFLAGS -x VendorID -x PyQt_NoPrintRangeBug -t ${ws_flag} -t Qt_${qt_flag} -g -s .cc -c .)
SET(PYQT_SIPFLAGS ${PYQT_SIPFLAGS} ${PYQT_INCLUDES})
