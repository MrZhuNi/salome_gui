TEMPLATE = lib
TARGET = Event
DESTDIR = ../../lib
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj/$$TARGET

KERNEL_CXXFLAGS = $$(KERNEL_ROOT_DIR)/include/salome

INCLUDEPATH += ../../include $${KERNEL_CXXFLAGS}
LIBS += 

CONFIG -= debug release debug_and_release
CONFIG += qt thread debug dll shared

win32:DEFINES += WIN32 
DEFINES += EVENT_EXPORTS

HEADERS  = Event.h
HEADERS += SALOME_Event.hxx

SOURCES  = SALOME_Event.cxx

includes.files = $$HEADERS
includes.path = ../../include

INSTALLS += includes
