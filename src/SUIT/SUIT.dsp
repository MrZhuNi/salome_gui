# Microsoft Developer Studio Project File - Name="SUIT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SUIT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SUIT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SUIT.mak" CFG="SUIT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SUIT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SUIT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SUIT"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SUIT - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\SUIT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SUIT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /D "SUIT_EXPORTS" /D "WNT" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\SUIT.dll" /libpath:"$(QTLIB)"

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\SUIT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SUIT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /D "SUIT_EXPORTS" /D "WNT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\SUIT.dll" /pdbtype:sept /libpath:"$(QTLIBD)"

!ENDIF 

# Begin Target

# Name "SUIT - Win32 Release"
# Name "SUIT - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SUIT_Accel.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ActionOperation.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Application.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_DataObject.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_DataObjectIterator.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_DataObjectKey.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_DataOwner.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Desktop.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ExceptionHandler.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_FileDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_FileValidator.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_MessageBox.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Operation.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_OverrideCursor.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ParserSettings.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_PopupClient.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ResourceMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_SelectionFilter.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_SelectionMgr.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Selector.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Session.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Study.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ToolButton.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_Tools.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=.\SUIT_ViewWindow.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SUIT.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT.h
InputName=SUIT

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT.h
InputName=SUIT

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_Accel.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Accel.h
InputName=SUIT_Accel

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Accel.h
InputName=SUIT_Accel

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

SOURCE=.\SUIT_ActionOperation.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ActionOperation.h
InputName=SUIT_ActionOperation

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ActionOperation.h
InputName=SUIT_ActionOperation

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

SOURCE=.\SUIT_Application.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Application.h
InputName=SUIT_Application

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Application.h
InputName=SUIT_Application

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

SOURCE=.\SUIT_Convertor.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_Convertor.h
InputName=SUIT_Convertor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_Convertor.h
InputName=SUIT_Convertor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_DataObject.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_DataObject.h
InputName=SUIT_DataObject

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_DataObject.h
InputName=SUIT_DataObject

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

SOURCE=.\SUIT_DataObjectIterator.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataObjectIterator.h
InputName=SUIT_DataObjectIterator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataObjectIterator.h
InputName=SUIT_DataObjectIterator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_DataObjectKey.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataObjectKey.h
InputName=SUIT_DataObjectKey

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataObjectKey.h
InputName=SUIT_DataObjectKey

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_DataOwner.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataOwner.h
InputName=SUIT_DataOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_DataOwner.h
InputName=SUIT_DataOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_Desktop.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Desktop.h
InputName=SUIT_Desktop

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Desktop.h
InputName=SUIT_Desktop

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

SOURCE=.\SUIT_ExceptionHandler.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ExceptionHandler.h
InputName=SUIT_ExceptionHandler

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ExceptionHandler.h
InputName=SUIT_ExceptionHandler

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_FileDlg.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_FileDlg.h
InputName=SUIT_FileDlg

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_FileDlg.h
InputName=SUIT_FileDlg

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

SOURCE=.\SUIT_FileValidator.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_FileValidator.h
InputName=SUIT_FileValidator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_FileValidator.h
InputName=SUIT_FileValidator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_MessageBox.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_MessageBox.h
InputName=SUIT_MessageBox

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_MessageBox.h
InputName=SUIT_MessageBox

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_Operation.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Operation.h
InputName=SUIT_Operation

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Operation.h
InputName=SUIT_Operation

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

SOURCE=.\SUIT_OverrideCursor.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_OverrideCursor.h
InputName=SUIT_OverrideCursor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_OverrideCursor.h
InputName=SUIT_OverrideCursor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_ParserSettings.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ParserSettings.h
InputName=SUIT_ParserSettings

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ParserSettings.h
InputName=SUIT_ParserSettings

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_PopupClient.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_PopupClient.h
InputName=SUIT_PopupClient

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_PopupClient.h
InputName=SUIT_PopupClient

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

SOURCE=.\SUIT_ResourceMgr.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ResourceMgr.h
InputName=SUIT_ResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_ResourceMgr.h
InputName=SUIT_ResourceMgr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_SelectionFilter.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_SelectionFilter.h
InputName=SUIT_SelectionFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_SelectionFilter.h
InputName=SUIT_SelectionFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_SelectionMgr.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_SelectionMgr.h
InputName=SUIT_SelectionMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_SelectionMgr.h
InputName=SUIT_SelectionMgr

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

SOURCE=.\SUIT_Selector.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Selector.h
InputName=SUIT_Selector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Selector.h
InputName=SUIT_Selector

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

SOURCE=.\SUIT_Session.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Session.h
InputName=SUIT_Session

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Session.h
InputName=SUIT_Session

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

SOURCE=.\SUIT_SmartPtr.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_SmartPtr.h
InputName=SUIT_SmartPtr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_SmartPtr.h
InputName=SUIT_SmartPtr

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_Study.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Study.h
InputName=SUIT_Study

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_Study.h
InputName=SUIT_Study

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

SOURCE=.\SUIT_ToolButton.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ToolButton.h
InputName=SUIT_ToolButton

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ToolButton.h
InputName=SUIT_ToolButton

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

SOURCE=.\SUIT_Tools.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_Tools.h
InputName=SUIT_Tools

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SUIT_Tools.h
InputName=SUIT_Tools

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SUIT_ViewManager.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewManager.h
InputName=SUIT_ViewManager

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewManager.h
InputName=SUIT_ViewManager

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

SOURCE=.\SUIT_ViewModel.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewModel.h
InputName=SUIT_ViewModel

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewModel.h
InputName=SUIT_ViewModel

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

SOURCE=.\SUIT_ViewWindow.h

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewWindow.h
InputName=SUIT_ViewWindow

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SUIT_ViewWindow.h
InputName=SUIT_ViewWindow

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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resources\cascade.png

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cascade.png
InputName=cascade

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cascade.png
InputName=cascade

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\htile.png

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\htile.png
InputName=htile

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\htile.png
InputName=htile

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\SUIT_images.po

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\SUIT_images.po
InputName=SUIT_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\SUIT_images.po
InputName=SUIT_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\SUIT_msg_en.po

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\SUIT_msg_en.po
InputName=SUIT_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\SUIT_msg_en.po
InputName=SUIT_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\vtile.png

!IF  "$(CFG)" == "SUIT - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\vtile.png
InputName=vtile

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "SUIT - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\vtile.png
InputName=vtile

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Accel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_ActionOperation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_DataObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Desktop.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_FileDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Operation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_PopupClient.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_SelectionMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Selector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Session.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_Study.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_ToolButton.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUIT_ViewWindow.cxx
# End Source File
# End Group
# End Target
# End Project
