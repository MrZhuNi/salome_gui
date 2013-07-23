# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/UseQT4EXT.cmake)

INCLUDE_DIRECTORIES(
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Qtx
  ${CMAKE_CURRENT_SOURCE_DIR}/../SUIT
  ${CMAKE_CURRENT_SOURCE_DIR}/../ImageComposer
)

SET(COMMON_LIBS
  ${QT_LIBRARIES} 
  suit 
  ImageComposer 
)

SET(GUI_HEADERS
  GraphicsView_PrsPropDlg.h
  GraphicsView_Scene.h
  GraphicsView_Selector.h
  GraphicsView_ViewFrame.h
  GraphicsView_ViewManager.h
  GraphicsView_ViewPort.h
  GraphicsView_Viewer.h
)

QT4_WRAP_CPP(GUI_HEADERS_MOC ${GUI_HEADERS})

SET(GraphicsView_SOURCES
  GraphicsView_Object.cxx
  GraphicsView_Scene.cxx
  GraphicsView_Selector.cxx
  GraphicsView_Viewer.cxx
  GraphicsView_ViewFrame.cxx
  GraphicsView_ViewManager.cxx
  GraphicsView_ViewPort.cxx
  GraphicsView_ViewTransformer.cxx
  GraphicsView_PrsImage.cxx
  GraphicsView_PrsImageFrame.cxx
  GraphicsView_PrsPropDlg.cxx
)

SET(GUITS_SOURCES
  resources/GraphicsView_images.ts
  resources/GraphicsView_msg_en.ts
  resources/GraphicsView_msg_fr.ts
)

ADD_LIBRARY(GraphicsView ${GraphicsView_SOURCES} ${GUI_HEADERS_MOC})
TARGET_LINK_LIBRARIES(GraphicsView ${COMMON_LIBS})
INSTALL(TARGETS GraphicsView DESTINATION ${GUI_salomelib_LIBS})

FILE(GLOB COMMON_HEADERS_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
INSTALL(FILES ${COMMON_HEADERS_H} DESTINATION ${GUI_salomeinclude_HEADERS})
QT4_INSTALL_TS_RESOURCES("${GUITS_SOURCES}" "${GUI_salomeres_DATA}")

FILE(GLOB GUIPNG_DATA "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.png")
INSTALL(FILES ${GUIPNG_DATA} DESTINATION ${GUI_salomeres_DATA})
