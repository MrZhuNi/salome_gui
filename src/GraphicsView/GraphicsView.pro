TEMPLATE = lib
TARGET = GraphicsView
DESTDIR = ../../lib
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj/$$TARGET

INCLUDEPATH += ../../include ../Qtx ../SUIT
LIBS += -L../../lib -lqtx -lsuit

CONFIG -= debug release debug_and_release
CONFIG += qt thread debug dll shared

win32:DEFINES += WIN32 
DEFINES += GRAPHICSVIEW_EXPORTS LIN LINTEL CSFDB No_exception HAVE_CONFIG_H HAVE_LIMITS_H

HEADERS  = GraphicsView.h
HEADERS += GraphicsView_Defs.h
HEADERS += GraphicsView_Scene.h
HEADERS += GraphicsView_Viewer.h
HEADERS += GraphicsView_ViewManager.h
HEADERS += GraphicsView_ViewTransformer.h
HEADERS += GraphicsView_Geom.h
HEADERS += GraphicsView_Object.h
HEADERS += GraphicsView_Selector.h
HEADERS += GraphicsView_ViewFrame.h
HEADERS += GraphicsView_ViewPort.h

SOURCES  = GraphicsView_Object.cxx
SOURCES += GraphicsView_Selector.cxx
SOURCES += GraphicsView_ViewFrame.cxx
SOURCES += GraphicsView_ViewPort.cxx
SOURCES += GraphicsView_Scene.cxx
SOURCES += GraphicsView_Viewer.cxx
SOURCES += GraphicsView_ViewManager.cxx
SOURCES += GraphicsView_ViewTransformer.cxx

TRANSLATIONS = resources/GraphicsView_images.ts \
               resources/GraphicsView_msg_en.ts \
               resources/GraphicsView_msg_fr.ts

ICONS   = resources/*.png

includes.files = $$HEADERS
includes.path = ../../include

resources.files = $$ICONS resources/*.qm
resources.path = ../../resources

INSTALLS += includes resources

