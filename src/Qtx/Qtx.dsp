# Microsoft Developer Studio Project File - Name="Qtx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Qtx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Qtx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Qtx.mak" CFG="Qtx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Qtx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Qtx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Qtx"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Qtx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\Qtx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "$(QTDIR)\include" /D "QTX_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\Qtx.dll" /libpath:"$(QTLIB)"

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\Qtx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(QTDIR)\include" /D "QTX_EXPORTS" /D "_DEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\Qtx.dll" /pdbtype:sept /libpath:"$(QTLIBD)"

!ENDIF 

# Begin Target

# Name "Qtx - Win32 Release"
# Name "Qtx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Qtx.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxAction.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxActionMenuMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxActionMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxActionToolMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxColorScale.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxComboBox.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxDblSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxDialog.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxDirListEditor.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxDockAction.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxDockWindow.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxGroupBox.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxIntSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxListAction.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxListBox.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxListOfOperations.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxListResourceEdit.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxListView.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxMainWindow.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxMenuButton.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxMRUAction.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxOperations.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxParser.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxPathDialog.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxPopupMenu.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxPopupMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxResourceEdit.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxResourceMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxStdOperations.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxTable.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxToolBar.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxToolTip.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxWorkspaceAction.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxWorkstack.cxx
# End Source File
# Begin Source File

SOURCE=.\QtxWorkstackAction.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Qtx.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Qtx.h
InputName=Qtx

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Qtx.h
InputName=Qtx

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxAction.h
InputName=QtxAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxAction.h
InputName=QtxAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxActionMenuMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionMenuMgr.h
InputName=QtxActionMenuMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionMenuMgr.h
InputName=QtxActionMenuMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxActionMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionMgr.h
InputName=QtxActionMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionMgr.h
InputName=QtxActionMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxActionToolMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionToolMgr.h
InputName=QtxActionToolMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxActionToolMgr.h
InputName=QtxActionToolMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxColorScale.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxColorScale.h
InputName=QtxColorScale

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxColorScale.h
InputName=QtxColorScale

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxComboBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxComboBox.h
InputName=QtxComboBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxComboBox.h
InputName=QtxComboBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxDblSpinBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDblSpinBox.h
InputName=QtxDblSpinBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDblSpinBox.h
InputName=QtxDblSpinBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxDialog.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDialog.h
InputName=QtxDialog

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDialog.h
InputName=QtxDialog

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxDirListEditor.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDirListEditor.h
InputName=QtxDirListEditor

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDirListEditor.h
InputName=QtxDirListEditor

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxDockAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDockAction.h
InputName=QtxDockAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDockAction.h
InputName=QtxDockAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxDockWindow.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDockWindow.h
InputName=QtxDockWindow

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxDockWindow.h
InputName=QtxDockWindow

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxGroupBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxGroupBox.h
InputName=QtxGroupBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxGroupBox.h
InputName=QtxGroupBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxIntSpinBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxIntSpinBox.h
InputName=QtxIntSpinBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxIntSpinBox.h
InputName=QtxIntSpinBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxListAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListAction.h
InputName=QtxListAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListAction.h
InputName=QtxListAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxListBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListBox.h
InputName=QtxListBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListBox.h
InputName=QtxListBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxListOfOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxListOfOperations.h
InputName=QtxListOfOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxListOfOperations.h
InputName=QtxListOfOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxListResourceEdit.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListResourceEdit.h
InputName=QtxListResourceEdit

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListResourceEdit.h
InputName=QtxListResourceEdit

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxListView.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListView.h
InputName=QtxListView

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxListView.h
InputName=QtxListView

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxMainWindow.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMainWindow.h
InputName=QtxMainWindow

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMainWindow.h
InputName=QtxMainWindow

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxMenuButton.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMenuButton.h
InputName=QtxMenuButton

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMenuButton.h
InputName=QtxMenuButton

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxMRUAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMRUAction.h
InputName=QtxMRUAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxMRUAction.h
InputName=QtxMRUAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxOperations.h
InputName=QtxOperations

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxOperations.h
InputName=QtxOperations

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxParser.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxParser.h
InputName=QtxParser

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxParser.h
InputName=QtxParser

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxPathDialog.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPathDialog.h
InputName=QtxPathDialog

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPathDialog.h
InputName=QtxPathDialog

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxPopupMenu.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPopupMenu.h
InputName=QtxPopupMenu

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPopupMenu.h
InputName=QtxPopupMenu

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxPopupMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPopupMgr.h
InputName=QtxPopupMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxPopupMgr.h
InputName=QtxPopupMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxResourceEdit.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxResourceEdit.h
InputName=QtxResourceEdit

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxResourceEdit.h
InputName=QtxResourceEdit

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxResourceMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxResourceMgr.h
InputName=QtxResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxResourceMgr.h
InputName=QtxResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxStdOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxStdOperations.h
InputName=QtxStdOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\QtxStdOperations.h
InputName=QtxStdOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxTable.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxTable.h
InputName=QtxTable

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxTable.h
InputName=QtxTable

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxToolBar.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxToolBar.h
InputName=QtxToolBar

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxToolBar.h
InputName=QtxToolBar

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxToolTip.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxToolTip.h
InputName=QtxToolTip

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxToolTip.h
InputName=QtxToolTip

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxWorkspaceAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkspaceAction.h
InputName=QtxWorkspaceAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkspaceAction.h
InputName=QtxWorkspaceAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxWorkstack.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkstack.h
InputName=QtxWorkstack

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkstack.h
InputName=QtxWorkstack

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QtxWorkstackAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkstackAction.h
InputName=QtxWorkstackAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\QtxWorkstackAction.h
InputName=QtxWorkstackAction

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_QtxAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxActionMenuMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxActionMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxActionToolMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxColorScale.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxComboBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxDblSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxDialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxDirListEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxDockAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxDockWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxGroupBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxIntSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxListAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxListBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxListResourceEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxListView.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxMainWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxMenuButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxMRUAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxOperations.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxParser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxPathDialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxPopupMenu.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxPopupMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxTable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxToolBar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxToolTip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxWorkspaceAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxWorkstack.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QtxWorkstackAction.cxx
# End Source File
# End Group
# End Target
# End Project
