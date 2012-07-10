unix:TEMPLATE = lib
win32:TEMPLATE = vclib

HEADERS = *.hxx *.ixx *.jxx

include(../Common.pro)

CAS_CPPFLAGS = $(CASINC)

unix:CAS_LDPATH = -L$(CASLIB) -lTKV3d
win32:CAS_LDPATH = -L$(CASLIB) -lTKernel -lTKMath -lTKV3d

INCLUDEPATH += $${CAS_CPPFLAGS}
LIBS += $${CAS_LDPATH}
win32:LIBS *= -L$(QTLIB)
win32:INCLUDEPATH *= $(QTINC) $(QTINC)\QtCore $(QTINC)\QtGui $(QTINC)\QtXml

win32:DEFINES += WNT WIN32 
DEFINES += $(CASDEFINES) OCC_VERSION_MAJOR=6 OCC_VERSION_MINOR=3 OCC_VERSION_MAINTENANCE=5 OCC_VERSION_LARGE=0x06050300 LIN LINTEL CSFDB No_Exception HAVE_CONFIG_H HAVE_LIMITS_H HAVE_WOK_CONFIG_H OCC_CONVERT_SIGNALS

