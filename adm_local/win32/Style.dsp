# Microsoft Developer Studio Project File - Name="Style" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Style - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Style.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Style.mak" CFG="Style - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Style - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Style - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Style"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Style - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\Style"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STYLE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /D "NDEBUG" /D "SALOMESTYLE_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\Style.dll" /libpath:"$(QTLIB)"
# Begin Custom Build - Copying $(TargetName).dll...
TargetDir=\Work\SysPipe\SUIT_3_1\win32\bin
TargetPath=\Work\SysPipe\SUIT_3_1\win32\bin\Style.dll
TargetName=Style
InputPath=\Work\SysPipe\SUIT_3_1\win32\bin\Style.dll
SOURCE="$(InputPath)"

"$(TargetDir)\styles\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /B $(TargetPath) $(TargetDir)\styles\$(TargetName).dll

# End Custom Build

!ELSEIF  "$(CFG)" == "Style - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\Style"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STYLE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /D "_DEBUG" /D "SALOMESTYLE_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt$(QTVER).lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\Style.dll" /pdbtype:sept /libpath:"$(QTLIBD)"
# Begin Custom Build - Copying $(TargetName).dll...
TargetDir=\Work\SysPipe\SUIT_3_1\win32\bind
TargetPath=\Work\SysPipe\SUIT_3_1\win32\bind\Style.dll
TargetName=Style
InputPath=\Work\SysPipe\SUIT_3_1\win32\bind\Style.dll
SOURCE="$(InputPath)"

"$(TargetDir)\styles\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /B $(TargetPath) $(TargetDir)\styles\$(TargetName).dll

# End Custom Build

!ENDIF 

# Begin Target

# Name "Style - Win32 Release"
# Name "Style - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\Style\SalomeStyle.h

!IF  "$(CFG)" == "Style - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Style\SalomeStyle.h
InputName=SalomeStyle

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Style - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\Style\SalomeStyle.h
InputName=SalomeStyle

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Style\SalomeStyle.cxx
# End Source File
# End Group
# End Target
# End Project
