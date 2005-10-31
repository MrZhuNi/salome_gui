# Microsoft Developer Studio Project File - Name="SVTK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SVTK - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SVTK.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SVTK.mak" CFG="SVTK - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SVTK - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SVTK - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SVTK"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SVTK - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\SVTK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SVTK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(VTKINC)" /I "$(OCCINC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SVTK_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 vtkParallel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKernel.lib vtkCommon.lib vtkRendering.lib vtkFiltering.lib vtkGraphics.lib vtkHybrid.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\SVTK.dll" /libpath:"$(QTLIB)" /libpath:"$(OCCLIB)" /libpath:"$(VTKLIB)"

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\SVTK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SVTK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(VTKINC)" /I "$(OCCINC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SVTK_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKernel.lib vtkCommon.lib vtkRendering.lib vtkFiltering.lib vtkGraphics.lib vtkHybrid.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\SVTK.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(OCCLIBD)" /libpath:"$(VTKLIB)"

!ENDIF 

# Begin Target

# Name "SVTK - Win32 Release"
# Name "SVTK - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SVTK_Actor.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_CubeAxesActor2D.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_InteractorStyle.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_Prs.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_RenderWindow.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_RenderWindowInteractor.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_Selector.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_Trihedron.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=.\SVTK_ViewWindow.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SVTK.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK.h
InputName=SVTK

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK.h
InputName=SVTK

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_Actor.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Actor.h
InputName=SVTK_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Actor.h
InputName=SVTK_Actor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_CubeAxesActor2D.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_CubeAxesActor2D.h
InputName=SVTK_CubeAxesActor2D

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_CubeAxesActor2D.h
InputName=SVTK_CubeAxesActor2D

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_Functor.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Functor.h
InputName=SVTK_Functor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Functor.h
InputName=SVTK_Functor

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_InteractorStyle.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_InteractorStyle.h
InputName=SVTK_InteractorStyle

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_InteractorStyle.h
InputName=SVTK_InteractorStyle

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

SOURCE=.\SVTK_Prs.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Prs.h
InputName=SVTK_Prs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Prs.h
InputName=SVTK_Prs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_RenderWindow.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_RenderWindow.h
InputName=SVTK_RenderWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_RenderWindow.h
InputName=SVTK_RenderWindow

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

SOURCE=.\SVTK_RenderWindowInteractor.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_RenderWindowInteractor.h
InputName=SVTK_RenderWindowInteractor

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_RenderWindowInteractor.h
InputName=SVTK_RenderWindowInteractor

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

SOURCE=.\SVTK_Selection.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Selection.h
InputName=SVTK_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Selection.h
InputName=SVTK_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_Selector.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Selector.h
InputName=SVTK_Selector

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Selector.h
InputName=SVTK_Selector

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_SelectorDef.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_SelectorDef.h
InputName=SVTK_SelectorDef

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_SelectorDef.h
InputName=SVTK_SelectorDef

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_Trihedron.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Trihedron.h
InputName=SVTK_Trihedron

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\SVTK_Trihedron.h
InputName=SVTK_Trihedron

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SVTK_ViewManager.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewManager.h
InputName=SVTK_ViewManager

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewManager.h
InputName=SVTK_ViewManager

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

SOURCE=.\SVTK_ViewModel.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewModel.h
InputName=SVTK_ViewModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewModel.h
InputName=SVTK_ViewModel

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

SOURCE=.\SVTK_ViewWindow.h

!IF  "$(CFG)" == "SVTK - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewWindow.h
InputName=SVTK_ViewWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SVTK - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\SVTK_ViewWindow.h
InputName=SVTK_ViewWindow

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
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_InteractorStyle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_RenderWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_RenderWindowInteractor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SVTK_ViewWindow.cxx
# End Source File
# End Group
# End Target
# End Project
