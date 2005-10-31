# Microsoft Developer Studio Project File - Name="GLViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GLViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GLViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GLViewer.mak" CFG="GLViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLViewer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GLViewer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "GLViewer"
# PROP Scc_LocalPath ".."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "opt"
# PROP Intermediate_Dir "opt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLVIEWER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WNT" /D "_USRDLL" /D "GLVIEWER_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib qt-mt333.lib tkernel.lib suit.lib gdi32.lib user32.lib /nologo /dll /machine:I386 /libpath:"$(QTDIR)\lib" /libpath:"$(OCCLIB)"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "deb"
# PROP Intermediate_Dir "deb"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLVIEWER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "WNT" /D "_USRDLL" /D "GLVIEWER_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib qt-mt333.lib tkernel.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /out:"../../win32/bind/GLViewer.dll" /implib:"../../win32/libd/GLViewer.lib" /pdbtype:sept /libpath:"$(QTDIR)\lib" /libpath:"$(OCCLIBD)"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "GLViewer - Win32 Release"
# Name "GLViewer - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GLViewer.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer.h
InputName=GLViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer.h
InputName=GLViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_AspectLine.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_AspectLine.h
InputName=GLViewer_AspectLine

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_AspectLine.h
InputName=GLViewer_AspectLine

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_BaseDrawers.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_BaseDrawers.h
InputName=GLViewer_BaseDrawers

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_BaseDrawers.h
InputName=GLViewer_BaseDrawers

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_BaseObjects.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_BaseObjects.h
InputName=GLViewer_BaseObjects

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_BaseObjects.h
InputName=GLViewer_BaseObjects

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Compass.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Compass.h
InputName=GLViewer_Compass

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Compass.h
InputName=GLViewer_Compass

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Context.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Context.h
InputName=GLViewer_Context

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Context.h
InputName=GLViewer_Context

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_CoordSystem.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_CoordSystem.h
InputName=GLViewer_CoordSystem

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_CoordSystem.h
InputName=GLViewer_CoordSystem

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Defs.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Defs.h
InputName=GLViewer_Defs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Defs.h
InputName=GLViewer_Defs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Drawer.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Drawer.h
InputName=GLViewer_Drawer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Drawer.h
InputName=GLViewer_Drawer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Geom.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Geom.h
InputName=GLViewer_Geom

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Geom.h
InputName=GLViewer_Geom

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Grid.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Grid.h
InputName=GLViewer_Grid

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Grid.h
InputName=GLViewer_Grid

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Group.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Group.h
InputName=GLViewer_Group

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Group.h
InputName=GLViewer_Group

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_MimeSource.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_MimeSource.h
InputName=GLViewer_MimeSource

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_MimeSource.h
InputName=GLViewer_MimeSource

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Object.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Object.h
InputName=GLViewer_Object

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Object.h
InputName=GLViewer_Object

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Selector.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Selector.h
InputName=GLViewer_Selector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Selector.h
InputName=GLViewer_Selector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Selector2d.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Selector2d.h
InputName=GLViewer_Selector2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Selector2d.h
InputName=GLViewer_Selector2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Text.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Text.h
InputName=GLViewer_Text

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Text.h
InputName=GLViewer_Text

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Tools.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Tools.h
InputName=GLViewer_Tools

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_Tools.h
InputName=GLViewer_Tools

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_ToolTip.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\GLViewer_ToolTip.h
InputName=GLViewer_ToolTip

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ToolTip.h
InputName=GLViewer_ToolTip

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Viewer.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Viewer.h
InputName=GLViewer_Viewer

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Viewer.h
InputName=GLViewer_Viewer

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Viewer2d.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Viewer2d.h
InputName=GLViewer_Viewer2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Viewer2d.h
InputName=GLViewer_Viewer2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewFrame.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewFrame.h
InputName=GLViewer_ViewFrame

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewFrame.h
InputName=GLViewer_ViewFrame

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewManager.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewManager.h
InputName=GLViewer_ViewManager

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewManager.h
InputName=GLViewer_ViewManager

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewPort.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewPort.h
InputName=GLViewer_ViewPort

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewPort.h
InputName=GLViewer_ViewPort

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewPort2d.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewPort2d.h
InputName=GLViewer_ViewPort2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_ViewPort2d.h
InputName=GLViewer_ViewPort2d

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GLViewer_Widget.h

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Widget.h
InputName=GLViewer_Widget

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\GLViewer_Widget.h
InputName=GLViewer_Widget

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
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

SOURCE=.\GLViewer_AspectLine.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_BaseDrawers.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_BaseObjects.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Compass.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Context.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_CoordSystem.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Drawer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Geom.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Grid.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Group.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_MimeSource.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Object.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Selector.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Selector2d.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Text.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Tools.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_ToolTip.cxx
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Viewer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Viewer2d.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewFrame.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewManager.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewPort.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_ViewPort2d.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GLViewer_Widget.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resources\GLViewer_images.po

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\GLViewer_images.po
InputName=GLViewer_images

"../../resources/$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)/bin/msg2qm resources/$(InputName).po ../../resources/$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\GLViewer_images.po
InputName=GLViewer_images

"../../resources/$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)/bin/msg2qm resources/$(InputName).po ../../resources/$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\GLViewer_msg_en.po

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\GLViewer_msg_en.po
InputName=GLViewer_msg_en

"../../resources/$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)/bin/msg2qm resources/$(InputName).po ../../resources/$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\GLViewer_msg_en.po
InputName=GLViewer_msg_en

"../../resources/$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)/bin/msg2qm resources/$(InputName).po ../../resources/$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_dump.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_dump.png
InputName=view_dump

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_dump.png
InputName=view_dump

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_fitall.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitall.png
InputName=view_fitall

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitall.png
InputName=view_fitall

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_fitarea.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitarea.png
InputName=view_fitarea

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitarea.png
InputName=view_fitarea

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_glpan.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_glpan.png
InputName=view_glpan

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_glpan.png
InputName=view_glpan

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_pan.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_pan.png
InputName=view_pan

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_pan.png
InputName=view_pan

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_reset.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_reset.png
InputName=view_reset

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_reset.png
InputName=view_reset

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_zoom.png

!IF  "$(CFG)" == "GLViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_zoom.png
InputName=view_zoom

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "GLViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_zoom.png
InputName=view_zoom

"../../resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ../../resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Meta Object Files"

# PROP Default_Filter "cpp;c;cxx"
# Begin Source File

SOURCE=.\moc\moc_GLViewer_Selector.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_Selector2d.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_ToolTip.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_Viewer.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_Viewer2d.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_ViewFrame.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_ViewPort.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_ViewPort2d.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\moc_GLViewer_Widget.cxx
# End Source File
# End Group
# End Target
# End Project
