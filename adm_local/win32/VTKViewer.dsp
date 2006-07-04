# Microsoft Developer Studio Project File - Name="VTKViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VTKViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VTKViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VTKViewer.mak" CFG="VTKViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VTKViewer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VTKViewer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "VTKViewer"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\VTKViewer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VTKVIEWER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /I "$(VTKINC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VTKVIEWER_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib vtkRendering.lib vtkCommon.lib vtkGraphics.lib vtkFiltering.lib tkernel.lib qt$(QTVER).lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\VTKViewer.dll" /libpath:"$(QTLIB)" /libpath:"$(OCCLIB)" /libpath:"$(VTKLIB)"

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\VTKViewer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VTKVIEWER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /I "$(VTKINC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VTKVIEWER_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib vtkRendering.lib vtkCommon.lib vtkGraphics.lib vtkFiltering.lib tkernel.lib qt$(QTVER).lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\VTKViewer.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(OCCLIBD)" /libpath:"$(VTKLIB)"

!ENDIF 

# Begin Target

# Name "VTKViewer - Win32 Release"
# Name "VTKViewer - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer.h
InputName=VTKViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer.h
InputName=VTKViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Actor.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Actor.h
InputName=VTKViewer_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Actor.h
InputName=VTKViewer_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Algorithm.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Algorithm.h
InputName=VTKViewer_Algorithm

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Algorithm.h
InputName=VTKViewer_Algorithm

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_CellRectPicker.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_CellRectPicker.h
InputName=VTKViewer_CellRectPicker

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_CellRectPicker.h
InputName=VTKViewer_CellRectPicker

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ConvexTool.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ConvexTool.h
InputName=VTKViewer_ConvexTool

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ConvexTool.h
InputName=VTKViewer_ConvexTool

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ExtractUnstructuredGrid.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ExtractUnstructuredGrid.h
InputName=VTKViewer_ExtractUnstructuredGrid

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ExtractUnstructuredGrid.h
InputName=VTKViewer_ExtractUnstructuredGrid

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Filter.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Filter.h
InputName=VTKViewer_Filter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Filter.h
InputName=VTKViewer_Filter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Functor.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Functor.h
InputName=VTKViewer_Functor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Functor.h
InputName=VTKViewer_Functor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_GeometryFilter.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_GeometryFilter.h
InputName=VTKViewer_GeometryFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_GeometryFilter.h
InputName=VTKViewer_GeometryFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_InteractorStyle.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_InteractorStyle.h
InputName=VTKViewer_InteractorStyle

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_InteractorStyle.h
InputName=VTKViewer_InteractorStyle

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_PassThroughFilter.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_PassThroughFilter.h
InputName=VTKViewer_PassThroughFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_PassThroughFilter.h
InputName=VTKViewer_PassThroughFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RectPicker.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_RectPicker.h
InputName=VTKViewer_RectPicker

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_RectPicker.h
InputName=VTKViewer_RectPicker

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RenderWindow.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_RenderWindow.h
InputName=VTKViewer_RenderWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_RenderWindow.h
InputName=VTKViewer_RenderWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RenderWindowInteractor.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_RenderWindowInteractor.h
InputName=VTKViewer_RenderWindowInteractor

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_RenderWindowInteractor.h
InputName=VTKViewer_RenderWindowInteractor

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ShrinkFilter.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ShrinkFilter.h
InputName=VTKViewer_ShrinkFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_ShrinkFilter.h
InputName=VTKViewer_ShrinkFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Transform.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Transform.h
InputName=VTKViewer_Transform

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Transform.h
InputName=VTKViewer_Transform

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_TransformFilter.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_TransformFilter.h
InputName=VTKViewer_TransformFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_TransformFilter.h
InputName=VTKViewer_TransformFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Trihedron.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Trihedron.h
InputName=VTKViewer_Trihedron

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Trihedron.h
InputName=VTKViewer_Trihedron

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Utilities.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Utilities.h
InputName=VTKViewer_Utilities

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_Utilities.h
InputName=VTKViewer_Utilities

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_VectorText.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_VectorText.h
InputName=VTKViewer_VectorText

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=..\..\src\VTKViewer\VTKViewer_VectorText.h
InputName=VTKViewer_VectorText

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewManager.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewManager.h
InputName=VTKViewer_ViewManager

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewManager.h
InputName=VTKViewer_ViewManager

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewModel.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewModel.h
InputName=VTKViewer_ViewModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewModel.h
InputName=VTKViewer_ViewModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewWindow.h

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewWindow.h
InputName=VTKViewer_ViewWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=..\..\src\VTKViewer\VTKViewer_ViewWindow.h
InputName=VTKViewer_ViewWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

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

SOURCE=..\..\src\VTKViewer\VTKViewer_Actor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_CellRectPicker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ConvexTool.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ExtractUnstructuredGrid.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Filter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_GeometryFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_InteractorStyle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_PassThroughFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RectPicker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RenderWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_RenderWindowInteractor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ShrinkFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Transform.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_TransformFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Trihedron.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_Utilities.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_VectorText.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\VTKViewer_ViewWindow.cxx
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_back.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_back.png
InputName=view_back

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_back.png
InputName=view_back

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_bottom.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_bottom.png
InputName=view_bottom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_bottom.png
InputName=view_bottom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_camera_dump.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_camera_dump.png
InputName=view_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_camera_dump.png
InputName=view_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_fitall.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_fitall.png
InputName=view_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_fitall.png
InputName=view_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_fitarea.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_fitarea.png
InputName=view_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_fitarea.png
InputName=view_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_front.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_front.png
InputName=view_front

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_front.png
InputName=view_front

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_glpan.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_glpan.png
InputName=view_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_glpan.png
InputName=view_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_left.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_left.png
InputName=view_left

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_left.png
InputName=view_left

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_pan.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_pan.png
InputName=view_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_pan.png
InputName=view_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_reset.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_reset.png
InputName=view_reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_reset.png
InputName=view_reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_right.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_right.png
InputName=view_right

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_right.png
InputName=view_right

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_rotate.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_rotate.png
InputName=view_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_rotate.png
InputName=view_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_top.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_top.png
InputName=view_top

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_top.png
InputName=view_top

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_triedre.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_triedre.png
InputName=view_triedre

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_triedre.png
InputName=view_triedre

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\view_zoom.png

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_zoom.png
InputName=view_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=..\..\src\VTKViewer\resources\view_zoom.png
InputName=view_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\VTKViewer_images.po

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\VTKViewer\resources\VTKViewer_images.po
InputName=VTKViewer_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\VTKViewer\resources\VTKViewer_images.po
InputName=VTKViewer_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\VTKViewer\resources\VTKViewer_msg_en.po

!IF  "$(CFG)" == "VTKViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\VTKViewer\resources\VTKViewer_msg_en.po
InputName=VTKViewer_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "VTKViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=..\..\src\VTKViewer\resources\VTKViewer_msg_en.po
InputName=VTKViewer_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Meta Object Files"

# PROP Default_Filter "*.cxx"
# Begin Source File

SOURCE=..\..\moc\moc_VTKViewer_RenderWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_VTKViewer_RenderWindowInteractor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_VTKViewer_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_VTKViewer_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_VTKViewer_ViewWindow.cxx
# End Source File
# End Group
# End Target
# End Project
