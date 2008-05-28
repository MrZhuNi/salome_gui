unix:TEMPLATE = lib
win32:TEMPLATE = vclib

DESTDIR = ../../$(CONFIG_ID)/lib
MOC_DIR = ../../moc
OBJECTS_DIR = ../../$(CONFIG_ID)/obj/$$TARGET

INCLUDEPATH += ../../include $$(PYTHONINC)
unix:LIBS  += -L$$(PYTHONLIB) -lpython2.5
win32:LIBS  += -L$$(PYTHONLIB) -lpython25_d

CONFIG -= debug release debug_and_release
CONFIG += qt thread debug dll shared

win32:DEFINES += WNT WIN32
DEFINES += PYINTERP_EXPORTS

HEADERS = *.h

SOURCES = *.cxx

win32:copy_hdr.name = Install ${QMAKE_FILE_IN}
win32:copy_hdr.commands = type ${QMAKE_FILE_IN} > ../../include/${QMAKE_FILE_BASE}.h
win32:copy_hdr.output = ../../include/${QMAKE_FILE_BASE}.h
win32:copy_hdr.input = HEADERS
win32:QMAKE_EXTRA_COMPILERS += copy_hdr

includes.files = $$HEADERS
includes.path = ../../include

INSTALLS += includes
