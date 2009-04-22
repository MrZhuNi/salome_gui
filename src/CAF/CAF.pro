unix:TEMPLATE = lib
win32:TEMPLATE = vclib

include(../Common.pro)

CAS_CPPFLAGS = $(CASINC)

CAS_KERNEL = -L$(CASLIB) -lTKernel

CAS_VIEWER = -L$(CASLIB) -lTKV3d -lTKService

INCLUDEPATH += $${CAS_CPPFLAGS}
LIBS += $${CAS_KERNEL} $${CAS_VIEWER} -lQtx -lSUIT -lSTD

win32:DEFINES += WIN32
DEFINES += $(CASDEFINES) CAF_EXPORTS OCC_VERSION_MAJOR=6 OCC_VERSION_MINOR=1 OCC_VERSION_MAINTENANCE=1 LIN LINTEL CSFDB No_exception HAVE_CONFIG_H HAVE_LIMITS_H HAVE_WOK_CONFIG_H OCC_CONVERT_SIGNALS

include(../Translations.pro)
include(../Resources.pro)

