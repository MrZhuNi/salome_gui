
INCLUDE(FindQt4)
INCLUDE(UseQt4)
SET(DIRS)
FOREACH(D ${QT_INCLUDES})
SET(DIRS ${DIRS} -I${D})
ENDFOREACH(D ${QT_INCLUDES})
SET(QT_INCLUDES ${DIRS})
SET(QT_INCLUDES ${QT_INCLUDES} -DQT_THREAD_SUPPORT)
SET(QT_MT_LIBS ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${QT_QTOPENGL_LIBRARY})

FIND_PROGRAM(QT_LRELEASE_EXECUTABLE lrelease)
