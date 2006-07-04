# Microsoft Developer Studio Project File - Name="QDS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=QDS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QDS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QDS.mak" CFG="QDS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QDS - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QDS - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "QDS"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QDS - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\QDS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "NDEBUG" /D "QDS_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib tkernel.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\QDS.dll" /libpath:"$(QTLIB)" /libpath:"$(OCCLIB)"

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\QDS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "_DEBUG" /D "QDS_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib tkernel.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\QDS.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(OCCLIBD)"

!ENDIF 

# Begin Target

# Name "QDS - Win32 Release"
# Name "QDS - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\QDS\QDS.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\QDS\QDS.h
InputName=QDS

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\QDS\QDS.h
InputName=QDS

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_CheckBox.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_CheckBox.h
InputName=QDS_CheckBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_CheckBox.h
InputName=QDS_CheckBox

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

SOURCE=..\..\src\QDS\QDS_ComboBox.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_ComboBox.h
InputName=QDS_ComboBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_ComboBox.h
InputName=QDS_ComboBox

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

SOURCE=..\..\src\QDS\QDS_Datum.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_Datum.h
InputName=QDS_Datum

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_Datum.h
InputName=QDS_Datum

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

SOURCE=..\..\src\QDS\QDS_LineEdit.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_LineEdit.h
InputName=QDS_LineEdit

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_LineEdit.h
InputName=QDS_LineEdit

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

SOURCE=..\..\src\QDS\QDS_RadioBox.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_RadioBox.h
InputName=QDS_RadioBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_RadioBox.h
InputName=QDS_RadioBox

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

SOURCE=..\..\src\QDS\QDS_SpinBox.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_SpinBox.h
InputName=QDS_SpinBox

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_SpinBox.h
InputName=QDS_SpinBox

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

SOURCE=..\..\src\QDS\QDS_SpinBoxDbl.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_SpinBoxDbl.h
InputName=QDS_SpinBoxDbl

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_SpinBoxDbl.h
InputName=QDS_SpinBoxDbl

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

SOURCE=..\..\src\QDS\QDS_TextEdit.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_TextEdit.h
InputName=QDS_TextEdit

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\QDS\QDS_TextEdit.h
InputName=QDS_TextEdit

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

SOURCE=..\..\src\QDS\QDS_Validator.h

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\QDS\QDS_Validator.h
InputName=QDS_Validator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\QDS\QDS_Validator.h
InputName=QDS_Validator

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\QDS\QDS.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_CheckBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_ComboBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_Datum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_LineEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_RadioBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_SpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_SpinBoxDbl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_TextEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\QDS\QDS_Validator.cxx
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "*.po *.png"
# Begin Source File

SOURCE=..\..\src\QDS\resources\QDS_msg_en.po

!IF  "$(CFG)" == "QDS - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\QDS\resources\QDS_msg_en.po
InputName=QDS_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "QDS - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\QDS\resources\QDS_msg_en.po
InputName=QDS_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Meta Object Files"

# PROP Default_Filter "*.cxx"
# Begin Source File

SOURCE=..\..\moc\moc_QDS_CheckBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_ComboBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_Datum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_LineEdit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_RadioBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_SpinBox.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_SpinBoxDbl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_QDS_TextEdit.cxx
# End Source File
# End Group
# End Target
# End Project
