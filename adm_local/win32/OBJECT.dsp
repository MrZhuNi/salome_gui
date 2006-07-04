# Microsoft Developer Studio Project File - Name="OBJECT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OBJECT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OBJECT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OBJECT.mak" CFG="OBJECT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OBJECT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OBJECT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "OBJECT"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\OBJECT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(PYTHONINC)" /I "$(OCCINC)" /I "$(VTKINC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECT_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKernel.lib TKV3d.lib vtkCommon.lib vtkRendering.lib vtkFiltering.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\OBJECT.dll" /libpath:"$(QTLIB)" /libpath:"$(OCCLIB)" /libpath:"$(PYTHONLIB)" /libpath:"$(VTKLIB)"

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\OBJECT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(PYTHONINC)" /I "$(OCCINC)" /I "$(VTKINC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECT_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKernel.lib TKV3d.lib vtkCommon.lib vtkRendering.lib vtkFiltering.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\OBJECT.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(OCCLIBD)" /libpath:"$(PYTHONLIB)" /libpath:"$(VTKLIB)"

!ENDIF 

# Begin Target

# Name "OBJECT - Win32 Release"
# Name "OBJECT - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_Actor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_AISObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_AISShape.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapOfIOMapOfInteger_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_Filter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_GLOwner.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_InteractiveObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListIO_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListIteratorOfListIO_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListNodeOfListIO_0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_TypeFilter.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_AISObject.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_AISObject.hxx
InputName=Handle_SALOME_AISObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_AISObject.hxx
InputName=Handle_SALOME_AISObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_AISShape.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_AISShape.hxx
InputName=Handle_SALOME_AISShape

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_AISShape.hxx
InputName=Handle_SALOME_AISShape

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx
InputName=Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx
InputName=Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_Filter.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_Filter.hxx
InputName=Handle_SALOME_Filter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_Filter.hxx
InputName=Handle_SALOME_Filter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_InteractiveObject.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_InteractiveObject.hxx
InputName=Handle_SALOME_InteractiveObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_InteractiveObject.hxx
InputName=Handle_SALOME_InteractiveObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_ListNodeOfListIO.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_ListNodeOfListIO.hxx
InputName=Handle_SALOME_ListNodeOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_ListNodeOfListIO.hxx
InputName=Handle_SALOME_ListNodeOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\Handle_SALOME_TypeFilter.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_TypeFilter.hxx
InputName=Handle_SALOME_TypeFilter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\Handle_SALOME_TypeFilter.hxx
InputName=Handle_SALOME_TypeFilter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_Actor.h

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_Actor.h
InputName=SALOME_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_Actor.h
InputName=SALOME_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_AISObject.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_AISObject.hxx
InputName=SALOME_AISObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_AISObject.hxx
InputName=SALOME_AISObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_AISShape.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_AISShape.hxx
InputName=SALOME_AISShape

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_AISShape.hxx
InputName=SALOME_AISShape

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapNodeOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapNodeOfDataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_DataMapOfIOMapOfInteger.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_DataMapOfIOMapOfInteger.hxx
InputName=SALOME_DataMapOfIOMapOfInteger

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_Filter.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_Filter.hxx
InputName=SALOME_Filter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_Filter.hxx
InputName=SALOME_Filter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_GLOwner.h

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_GLOwner.h
InputName=SALOME_GLOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_GLOwner.h
InputName=SALOME_GLOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_InteractiveObject.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_InteractiveObject.hxx
InputName=SALOME_InteractiveObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_InteractiveObject.hxx
InputName=SALOME_InteractiveObject

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListIO.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListIO.hxx
InputName=SALOME_ListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListIO.hxx
InputName=SALOME_ListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListIteratorOfListIO.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListIteratorOfListIO.hxx
InputName=SALOME_ListIteratorOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListIteratorOfListIO.hxx
InputName=SALOME_ListIteratorOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_ListNodeOfListIO.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListNodeOfListIO.hxx
InputName=SALOME_ListNodeOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_ListNodeOfListIO.hxx
InputName=SALOME_ListNodeOfListIO

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_Selection.h

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_Selection.h
InputName=SALOME_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\OBJECT\SALOME_Selection.h
InputName=SALOME_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OBJECT\SALOME_TypeFilter.hxx

!IF  "$(CFG)" == "OBJECT - Win32 Release"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_TypeFilter.hxx
InputName=SALOME_TypeFilter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ELSEIF  "$(CFG)" == "OBJECT - Win32 Debug"

# Begin Custom Build - Update $(InputName).hxx...
InputPath=..\..\src\OBJECT\SALOME_TypeFilter.hxx
InputName=SALOME_TypeFilter

"..\..\include\$(InputName).hxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).hxx

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
