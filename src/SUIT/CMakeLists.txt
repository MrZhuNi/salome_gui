PROJECT(SUIT)

INCLUDE(${QT_USE_FILE})

#
# Headers
SET(Suit_INCLUDE_FILES
  SUIT_Accel.h                                                                                                                               
  SUIT_ActionOperation.h                                                                                                                     
  SUIT_Application.h                                                                                                                         
  SUIT_DataBrowser.h
  SUIT_DataObject.h
  SUIT_DataObjectIterator.h
  SUIT_DataObjectKey.h
  SUIT_DataOwner.h
  SUIT_Desktop.h
  SUIT_ExceptionHandler.h
  SUIT_FileDlg.h
  SUIT_FileValidator.h
  SUIT.h
  SUIT_LicenseDlg.h
  SUIT_MessageBox.h
  SUIT_Operation.h
  SUIT_OverrideCursor.h
  SUIT_PopupClient.h
  SUIT_PreferenceMgr.h
  SUIT_ResourceMgr.h
  SUIT_SelectionFilter.h
  SUIT_SelectionMgr.h
  SUIT_Selector.h
  SUIT_Session.h
  SUIT_SmartPtr.h
  SUIT_Study.h
  SUIT_Tools.h
  SUIT_TreeModel.h
  SUIT_TreeSync.h
  SUIT_ViewManager.h
  SUIT_ViewModel.h
  SUIT_ViewWindow.h)

#
# MOC sources
SET(Suit_MOC_SOURCES 
  SUIT_Accel.h                                                                                                                               
  SUIT_ActionOperation.h                                                                                                                     
  SUIT_Application.h                                                                                                                         
  SUIT_DataBrowser.h
  SUIT_DataObject.h
  SUIT_Desktop.h
  SUIT_FileDlg.h
  SUIT_LicenseDlg.h
  SUIT_Operation.h
  SUIT_PopupClient.h
  SUIT_PreferenceMgr.h
  SUIT_SelectionMgr.h
  SUIT_Session.h
  SUIT_Study.h
  SUIT_TreeModel.h
  SUIT_ViewManager.h
  SUIT_ViewModel.h
  SUIT_ViewWindow.h)

#
# Source files
SET(Suit_SOURCE_FILES
  SUIT_Accel.cxx                                                                                                                             
  SUIT_ActionOperation.cxx
  SUIT_Application.cxx
  SUIT_DataBrowser.cxx
  SUIT_DataObject.cxx
  SUIT_DataObjectIterator.cxx
  SUIT_DataObjectKey.cxx
  SUIT_DataOwner.cxx
  SUIT_Desktop.cxx
  SUIT_ExceptionHandler.cxx
  SUIT_FileDlg.cxx
  SUIT_FileValidator.cxx
  SUIT_LicenseDlg.cxx
  SUIT_MessageBox.cxx
  SUIT_Operation.cxx
  SUIT_OverrideCursor.cxx
  SUIT_PopupClient.cxx
  SUIT_PreferenceMgr.cxx
  SUIT_ResourceMgr.cxx
  SUIT_SelectionFilter.cxx
  SUIT_SelectionMgr.cxx
  SUIT_Selector.cxx
  SUIT_Session.cxx
  SUIT_Study.cxx
  SUIT_Tools.cxx
  SUIT_TreeModel.cxx
  SUIT_ViewManager.cxx
  SUIT_ViewModel.cxx
  SUIT_ViewWindow.cxx)

#
# resources files
SET(resource_FILES  SUIT_msg_en)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../Qtx ${CMAKE_CURRENT_SOURCE_DIR}/../ObjBrowser)

ADD_DEFINITIONS( -DSUIT_EXPORTS )

QT4_WRAP_CPP(MOC_BUILT_SOURCES ${Suit_MOC_SOURCES})


ADD_LIBRARY(suit SHARED
  ${Suit_SOURCE_FILES}
  ${MOC_BUILT_SOURCES} )

TARGET_LINK_LIBRARIES(suit qtx objbrowser )

#
# Installations
#

INSTALL(TARGETS suit
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${Suit_INCLUDE_FILES}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

SET(input_name ${CMAKE_CURRENT_SOURCE_DIR}/resources/${resource_FILES}.ts)
SET(output_name ${CMAKE_CURRENT_BINARY_DIR}/${resource_FILES}.qm)

INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${input_name} -qm ${output_name})")
INSTALL(FILES ${output_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/resources)