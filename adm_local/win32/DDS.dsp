# Microsoft Developer Studio Project File - Name="DDS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DDS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DDS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DDS.mak" CFG="DDS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DDS - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DDS - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "DDS"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DDS - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\DDS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /I "..\..\include" /I "$(OCCINC)" /D "QTX_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tkernel.lib tkcdf.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\DDS.dll" /libpath:"$(OCCLIB)"

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\DDS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QTX_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(OCCINC)" /D "QTX_EXPORTS" /D "_DEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tkernel.lib tkcdf.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\DDS.dll" /pdbtype:sept /libpath:"$(OCCLIBD)"

!ENDIF 

# Begin Target

# Name "DDS - Win32 Release"
# Name "DDS - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\DDS\DDS.h

!IF  "$(CFG)" == "DDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS.h
InputName=DDS

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS.h
InputName=DDS

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_DicGroup.h

!IF  "$(CFG)" == "DDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_DicGroup.h
InputName=DDS_DicGroup

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_DicGroup.h
InputName=DDS_DicGroup

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_DicItem.h

!IF  "$(CFG)" == "DDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_DicItem.h
InputName=DDS_DicItem

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_DicItem.h
InputName=DDS_DicItem

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_Dictionary.h

!IF  "$(CFG)" == "DDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_Dictionary.h
InputName=DDS_Dictionary

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_Dictionary.h
InputName=DDS_Dictionary

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_KeyWords.h

!IF  "$(CFG)" == "DDS - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_KeyWords.h
InputName=DDS_KeyWords

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "DDS - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\DDS\DDS_KeyWords.h
InputName=DDS_KeyWords

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\DDS\DDS_DicGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_DicItem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_Dictionary.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\DDS\DDS_KeyWords.cxx
# End Source File
# End Group
# End Target
# End Project
