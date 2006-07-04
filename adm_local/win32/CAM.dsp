# Microsoft Developer Studio Project File - Name="CAM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CAM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CAM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAM.mak" CFG="CAM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CAM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CAM"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CAM - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\CAM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAM_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /D "CAM_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\CAM.dll" /libpath:"$(QTLIB)"

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\CAM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAM_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /D "CAM_EXPORTS" /D "_DEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\CAM.dll" /pdbtype:sept /libpath:"$(QTLIBD)"

!ENDIF 

# Begin Target

# Name "CAM - Win32 Release"
# Name "CAM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_DataModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_DataObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Module.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_RootObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Study.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\CAM\CAM.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM.h
InputName=CAM

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM.h
InputName=CAM

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Application.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Application.h
InputName=CAM_Application

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Application.h
InputName=CAM_Application

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_DataModel.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_DataModel.h
InputName=CAM_DataModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_DataModel.h
InputName=CAM_DataModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_DataObject.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM_DataObject.h
InputName=CAM_DataObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM_DataObject.h
InputName=CAM_DataObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Module.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Module.h
InputName=CAM_Module

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Module.h
InputName=CAM_Module

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_RootObject.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM_RootObject.h
InputName=CAM_RootObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\CAM\CAM_RootObject.h
InputName=CAM_RootObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\CAM\CAM_Study.h

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Study.h
InputName=CAM_Study

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\CAM\CAM_Study.h
InputName=CAM_Study

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\src\CAM\resources\CAM_msg_en.po

!IF  "$(CFG)" == "CAM - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\CAM\resources\CAM_msg_en.po
InputName=CAM_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "CAM - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\CAM\resources\CAM_msg_en.po
InputName=CAM_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_CAM_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_CAM_DataModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_CAM_Module.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_CAM_Study.cxx
# End Source File
# End Group
# End Target
# End Project
