unix:TEMPLATE = lib
win32:TEMPLATE = vclib

HEADERS = *.hxx
include(../Common.pro)

CAS_CPPFLAGS = $(CASINC)

CAS_KERNEL = -L$(CASLIB) -lTKernel

CAS_VIEWER = -L$(CASLIB) -lTKV3d -lTKService

INCLUDEPATH += $${CAS_CPPFLAGS}
LIBS += $${CAS_KERNEL} $${CAS_VIEWER}
win32:LIBS *= -L$(QTLIB)
win32:INCLUDEPATH *= $(QTINC) $(QTINC)\QtCore $(QTINC)\QtGui $(QTINC)\QtXml

win32:DEFINES += WNT WIN32 
DEFINES += HTMLSERVICE_EXPORTS $(CASDEFINES) OCC_VERSION_MAJOR=6 OCC_VERSION_MINOR=3 OCC_VERSION_MAINTENANCE=5 LIN LINTEL CSFDB No_Exception HAVE_CONFIG_H HAVE_LIMITS_H HAVE_WOK_CONFIG_H OCC_CONVERT_SIGNALS