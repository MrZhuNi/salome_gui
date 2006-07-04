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
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Qtx - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\Qtx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /I "$(QTDIR)\include" /D "QTX_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\Qtx.dll" /libpath:"$(QTLIB)"

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\Qtx.dll" /pdbtype:sept /libpath:"$(QTLIBD)"

!ENDIF 

# Begin Target

# Name "Qtx - Win32 Release"
# Name "Qtx - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\Qtx\Qtx.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\Qtx.h
InputName=Qtx

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\Qtx.h
InputName=Qtx

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxAction.h
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
InputPath=..\..\src\Qtx\QtxAction.h
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

SOURCE=..\..\src\Qtx\QtxActionMenuMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxActionMenuMgr.h
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
InputPath=..\..\src\Qtx\QtxActionMenuMgr.h
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

SOURCE=..\..\src\Qtx\QtxActionMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxActionMgr.h
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
InputPath=..\..\src\Qtx\QtxActionMgr.h
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

SOURCE=..\..\src\Qtx\QtxActionToolMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxActionToolMgr.h
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
InputPath=..\..\src\Qtx\QtxActionToolMgr.h
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

SOURCE=..\..\src\Qtx\QtxColorScale.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxColorScale.h
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
InputPath=..\..\src\Qtx\QtxColorScale.h
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

SOURCE=..\..\src\Qtx\QtxComboBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxComboBox.h
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
InputPath=..\..\src\Qtx\QtxComboBox.h
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

SOURCE=..\..\src\Qtx\QtxDblSpinBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxDblSpinBox.h
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
InputPath=..\..\src\Qtx\QtxDblSpinBox.h
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

SOURCE=..\..\src\Qtx\QtxDialog.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxDialog.h
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
InputPath=..\..\src\Qtx\QtxDialog.h
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

SOURCE=..\..\src\Qtx\QtxDirListEditor.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxDirListEditor.h
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
InputPath=..\..\src\Qtx\QtxDirListEditor.h
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

SOURCE=..\..\src\Qtx\QtxDockAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxDockAction.h
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
InputPath=..\..\src\Qtx\QtxDockAction.h
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

SOURCE=..\..\src\Qtx\QtxDockWindow.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxDockWindow.h
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
InputPath=..\..\src\Qtx\QtxDockWindow.h
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

SOURCE=..\..\src\Qtx\QtxGroupBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxGroupBox.h
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
InputPath=..\..\src\Qtx\QtxGroupBox.h
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

SOURCE=..\..\src\Qtx\QtxIntSpinBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxIntSpinBox.h
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
InputPath=..\..\src\Qtx\QtxIntSpinBox.h
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

SOURCE=..\..\src\Qtx\QtxListAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxListAction.h
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
InputPath=..\..\src\Qtx\QtxListAction.h
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

SOURCE=..\..\src\Qtx\QtxListBox.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxListBox.h
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
InputPath=..\..\src\Qtx\QtxListBox.h
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

SOURCE=..\..\src\Qtx\QtxListOfOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxListOfOperations.h
InputName=QtxListOfOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxListOfOperations.h
InputName=QtxListOfOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListResourceEdit.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxListResourceEdit.h
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
InputPath=..\..\src\Qtx\QtxListResourceEdit.h
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

SOURCE=..\..\src\Qtx\QtxListView.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxListView.h
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
InputPath=..\..\src\Qtx\QtxListView.h
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

SOURCE=..\..\src\Qtx\QtxLogoMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxLogoMgr.h
InputName=QtxLogoMgr

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
InputPath=..\..\src\Qtx\QtxLogoMgr.h
InputName=QtxLogoMgr

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

SOURCE=..\..\src\Qtx\QtxMainWindow.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxMainWindow.h
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
InputPath=..\..\src\Qtx\QtxMainWindow.h
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

SOURCE=..\..\src\Qtx\QtxMenuButton.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxMenuButton.h
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
InputPath=..\..\src\Qtx\QtxMenuButton.h
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

SOURCE=..\..\src\Qtx\QtxMRUAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxMRUAction.h
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
InputPath=..\..\src\Qtx\QtxMRUAction.h
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

SOURCE=..\..\src\Qtx\QtxOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxOperations.h
InputName=QtxOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxOperations.h
InputName=QtxOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxParser.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxParser.h
InputName=QtxParser

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxParser.h
InputName=QtxParser

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxPathDialog.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxPathDialog.h
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
InputPath=..\..\src\Qtx\QtxPathDialog.h
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

SOURCE=..\..\src\Qtx\QtxPopupMenu.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxPopupMenu.h
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
InputPath=..\..\src\Qtx\QtxPopupMenu.h
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

SOURCE=..\..\src\Qtx\QtxPopupMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxPopupMgr.h
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
InputPath=..\..\src\Qtx\QtxPopupMgr.h
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

SOURCE=..\..\src\Qtx\QtxResourceEdit.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxResourceEdit.h
InputName=QtxResourceEdit

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxResourceEdit.h
InputName=QtxResourceEdit

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxResourceMgr.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxResourceMgr.h
InputName=QtxResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxResourceMgr.h
InputName=QtxResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxStdOperations.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxStdOperations.h
InputName=QtxStdOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Qtx - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Qtx\QtxStdOperations.h
InputName=QtxStdOperations

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxTable.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxTable.h
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
InputPath=..\..\src\Qtx\QtxTable.h
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

SOURCE=..\..\src\Qtx\QtxToolBar.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxToolBar.h
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
InputPath=..\..\src\Qtx\QtxToolBar.h
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

SOURCE=..\..\src\Qtx\QtxToolTip.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxToolTip.h
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
InputPath=..\..\src\Qtx\QtxToolTip.h
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

SOURCE=..\..\src\Qtx\QtxWorkspaceAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxWorkspaceAction.h
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
InputPath=..\..\src\Qtx\QtxWorkspaceAction.h
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

SOURCE=..\..\src\Qtx\QtxWorkstack.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxWorkstack.h
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
InputPath=..\..\src\Qtx\QtxWorkstack.h
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

SOURCE=..\..\src\Qtx\QtxWorkstackAction.h

!IF  "$(CFG)" == "Qtx - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\Qtx\QtxWorkstackAction.h
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
InputPath=..\..\src\Qtx\QtxWorkstackAction.h
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
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Qtx\Qtx.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxActionMenuMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxActionMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxActionToolMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxColorScale.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxComboBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxDblSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxDialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxDirListEditor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxDockAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxDockWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxGroupBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxIntSpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListOfOperations.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListResourceEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxListView.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxLogoMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxMainWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxMenuButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxMRUAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxOperations.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxParser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxPathDialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxPopupMenu.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxPopupMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxResourceEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxResourceMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxStdOperations.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxTable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxToolBar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxToolTip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxWorkspaceAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxWorkstack.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Qtx\QtxWorkstackAction.cxx
# End Source File
# End Group
# Begin Group "Meta Object Files"

# PROP Default_Filter "*.cxx"
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

SOURCE=..\..\moc\moc_QtxLogoMgr.cxx
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
