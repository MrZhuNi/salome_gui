TEMPLATE = app

DESTDIR = ../../$(CONFIG_ID)/bin
MOC_DIR = ../../moc
OBJECTS_DIR = ../../$(CONFIG_ID)/obj/$$TARGET

INCLUDEPATH = ../../include
LIBS += -L../../$(CONFIG_ID)/lib -lSUIT -lQtx -lStyle -lObjBrowser

CONFIG -= debug release debug_and_release
CONFIG += qt thread debug dll shared

win32:DEFINES += WIN32
DEFINES += DISABLE_TESTRECORDER

HEADERS = *.h

SOURCES = *.cxx
