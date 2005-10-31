# Microsoft Developer Studio Project File - Name="OCCViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OCCViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OCCViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OCCViewer.mak" CFG="OCCViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OCCViewer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OCCViewer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "OCCViewer"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\OCCViewer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OCCVIEWER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OCCVIEWER_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKV3d.lib TKernel.lib TKService.lib TKG3d.lib TKMath.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\OCCViewer.dll" /libpath:"$(QTLIB)" /libpath:"$(OCCLIB)"

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\OCCViewer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OCCVIEWER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OCCVIEWER_EXPORTS" /D "WNT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib TKV3d.lib TKernel.lib TKService.lib TKG3d.lib TKMath.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\OCCViewer.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(OCCLIBD)"

!ENDIF 

# Begin Target

# Name "OCCViewer - Win32 Release"
# Name "OCCViewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OCCViewer_AISSelector.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_CreateRestoreViewDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_ViewPort.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_ViewPort3d.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_ViewWindow.cxx
# End Source File
# Begin Source File

SOURCE=.\OCCViewer_VService.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OCCViewer.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\OCCViewer.h
InputName=OCCViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\OCCViewer.h
InputName=OCCViewer

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OCCViewer_AISSelector.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_AISSelector.h
InputName=OCCViewer_AISSelector

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_AISSelector.h
InputName=OCCViewer_AISSelector

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

SOURCE=.\OCCViewer_CreateRestoreViewDlg.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_CreateRestoreViewDlg.h
InputName=OCCViewer_CreateRestoreViewDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_CreateRestoreViewDlg.h
InputName=OCCViewer_CreateRestoreViewDlg

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

SOURCE=.\OCCViewer_ViewManager.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewManager.h
InputName=OCCViewer_ViewManager

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewManager.h
InputName=OCCViewer_ViewManager

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

SOURCE=.\OCCViewer_ViewModel.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewModel.h
InputName=OCCViewer_ViewModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewModel.h
InputName=OCCViewer_ViewModel

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

SOURCE=.\OCCViewer_ViewPort.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewPort.h
InputName=OCCViewer_ViewPort

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewPort.h
InputName=OCCViewer_ViewPort

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

SOURCE=.\OCCViewer_ViewPort3d.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewPort3d.h
InputName=OCCViewer_ViewPort3d

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewPort3d.h
InputName=OCCViewer_ViewPort3d

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

SOURCE=.\OCCViewer_ViewWindow.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewWindow.h
InputName=OCCViewer_ViewWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\OCCViewer_ViewWindow.h
InputName=OCCViewer_ViewWindow

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

SOURCE=.\OCCViewer_VService.h

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\OCCViewer_VService.h
InputName=OCCViewer_VService

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\OCCViewer_VService.h
InputName=OCCViewer_VService

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resources\OCCViewer_images.po

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\OCCViewer_images.po
InputName=OCCViewer_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\OCCViewer_images.po
InputName=OCCViewer_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\OCCViewer_msg_en.po

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\OCCViewer_msg_en.po
InputName=OCCViewer_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\OCCViewer_msg_en.po
InputName=OCCViewer_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_back.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_back.png
InputName=view_back

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_back.png
InputName=view_back

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_bottom.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_bottom.png
InputName=view_bottom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_bottom.png
InputName=view_bottom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_camera_dump.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_camera_dump.png
InputName=view_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_camera_dump.png
InputName=view_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_clone.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_clone.png
InputName=view_clone

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_clone.png
InputName=view_clone

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_fitall.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitall.png
InputName=view_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitall.png
InputName=view_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_fitarea.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitarea.png
InputName=view_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_fitarea.png
InputName=view_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_front.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_front.png
InputName=view_front

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_front.png
InputName=view_front

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_glpan.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_glpan.png
InputName=view_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_glpan.png
InputName=view_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_left.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_left.png
InputName=view_left

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_left.png
InputName=view_left

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_pan.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_pan.png
InputName=view_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_pan.png
InputName=view_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_presets.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_presets.png
InputName=view_presets

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_presets.png
InputName=view_presets

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_reset.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_reset.png
InputName=view_reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_reset.png
InputName=view_reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_right.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_right.png
InputName=view_right

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_right.png
InputName=view_right

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_rotate.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_rotate.png
InputName=view_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_rotate.png
InputName=view_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_shoot.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_shoot.png
InputName=view_shoot

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_shoot.png
InputName=view_shoot

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_top.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_top.png
InputName=view_top

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_top.png
InputName=view_top

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_triedre.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_triedre.png
InputName=view_triedre

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_triedre.png
InputName=view_triedre

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\view_zoom.png

!IF  "$(CFG)" == "OCCViewer - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_zoom.png
InputName=view_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "OCCViewer - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\view_zoom.png
InputName=view_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_AISSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_CreateRestoreViewDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_ViewPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_ViewPort3d.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_OCCViewer_ViewWindow.cxx
# End Source File
# End Group
# End Target
# End Project
