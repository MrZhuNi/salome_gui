TEMPLATE = lib
TARGET = RegistryDisplay
DESTDIR = ../../lib
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj/$$TARGET

KERNEL_CXXFLAGS = $$(KERNEL_ROOT_DIR)/include/salome

CORBA_INCLUDES = $$(OMNIORBDIR)/include $$(OMNIORBDIR)/include/omniORB4 $$(OMNIORBDIR)/include/COS

KERNEL_LDFLAGS = $$(KERNEL_ROOT_DIR)/lib/salome

INCLUDEPATH += ../../salome_adm/unix ../../idl $${KERNEL_CXXFLAGS} $${CORBA_INCLUDES}
LIBS += -L$${KERNEL_LDFLAGS} -lSalomeNS -lOpUtil -lSalomeIDLKernel

CONFIG -= debug release debug_and_release
CONFIG += qt thread debug dll shared

win32:DEFINES += WIN32 
DEFINES += OMNIORB_VERSION=4 __x86__ __linux__ COMP_CORBA_DOUBLE COMP_CORBA_LONG

HEADERS  = HelpWindow.hxx
HEADERS += IntervalWindow.hxx
HEADERS += RegWidget.h

SOURCES  = HelpWindow.cxx
SOURCES += RegWidget.cxx
SOURCES += RegWidgetFactory.cxx
SOURCES += IntervalWindow.cxx

includes.files = $$HEADERS
includes.path = ../../include

INSTALLS += includes
