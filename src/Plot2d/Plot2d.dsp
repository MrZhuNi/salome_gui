# Microsoft Developer Studio Project File - Name="Plot2d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Plot2d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Plot2d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Plot2d.mak" CFG="Plot2d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Plot2d - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Plot2d - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Plot2d"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\Plot2d"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLOT2D_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(QWTINC)" /D "PLOT2D_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib qwt.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\Plot2d.dll" /libpath:"$(QTLIB)" /libpath:"$(QWTLIB)"

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\Plot2d"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLOT2D_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(QWTINC)" /D "PLOT2D_EXPORTS" /D "_DEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib qwt.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\Plot2d.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(QWTLIBD)"

!ENDIF 

# Begin Target

# Name "Plot2d - Win32 Release"
# Name "Plot2d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Plot2d_Curve.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_FitDataDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_Prs.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_SetupCurveDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_SetupViewDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_ViewFrame.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=.\Plot2d_ViewWindow.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Plot2d.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d.h
InputName=Plot2d

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d.h
InputName=Plot2d

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plot2d_Curve.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d_Curve.h
InputName=Plot2d_Curve

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d_Curve.h
InputName=Plot2d_Curve

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plot2d_FitDataDlg.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_FitDataDlg.h
InputName=Plot2d_FitDataDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_FitDataDlg.h
InputName=Plot2d_FitDataDlg

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

SOURCE=.\Plot2d_Prs.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d_Prs.h
InputName=Plot2d_Prs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\Plot2d_Prs.h
InputName=Plot2d_Prs

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plot2d_SetupCurveDlg.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_SetupCurveDlg.h
InputName=Plot2d_SetupCurveDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_SetupCurveDlg.h
InputName=Plot2d_SetupCurveDlg

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

SOURCE=.\Plot2d_SetupViewDlg.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_SetupViewDlg.h
InputName=Plot2d_SetupViewDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_SetupViewDlg.h
InputName=Plot2d_SetupViewDlg

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

SOURCE=.\Plot2d_ViewFrame.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewFrame.h
InputName=Plot2d_ViewFrame

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewFrame.h
InputName=Plot2d_ViewFrame

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

SOURCE=.\Plot2d_ViewManager.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewManager.h
InputName=Plot2d_ViewManager

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewManager.h
InputName=Plot2d_ViewManager

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

SOURCE=.\Plot2d_ViewModel.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewModel.h
InputName=Plot2d_ViewModel

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewModel.h
InputName=Plot2d_ViewModel

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

SOURCE=.\Plot2d_ViewWindow.h

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewWindow.h
InputName=Plot2d_ViewWindow

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\Plot2d_ViewWindow.h
InputName=Plot2d_ViewWindow

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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resources\plot2d_camera_dump.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_camera_dump.png
InputName=plot2d_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_camera_dump.png
InputName=plot2d_camera_dump

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_clone.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_clone.png
InputName=plot2d_clone

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_clone.png
InputName=plot2d_clone

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_fitall.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_fitall.png
InputName=plot2d_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_fitall.png
InputName=plot2d_fitall

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_fitarea.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_fitarea.png
InputName=plot2d_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_fitarea.png
InputName=plot2d_fitarea

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_glpan.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_glpan.png
InputName=plot2d_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_glpan.png
InputName=plot2d_glpan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\Plot2d_images.po

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\Plot2d_images.po
InputName=Plot2d_images

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\Plot2d_images.po
InputName=Plot2d_images

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_legend.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_legend.png
InputName=plot2d_legend

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_legend.png
InputName=plot2d_legend

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_linear.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_linear.png
InputName=plot2d_linear

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_linear.png
InputName=plot2d_linear

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_linear_y.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_linear_y.png
InputName=plot2d_linear_y

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_linear_y.png
InputName=plot2d_linear_y

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_lines.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_lines.png
InputName=plot2d_lines

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_lines.png
InputName=plot2d_lines

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_log.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_log.png
InputName=plot2d_log

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_log.png
InputName=plot2d_log

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_log_y.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_log_y.png
InputName=plot2d_log_y

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_log_y.png
InputName=plot2d_log_y

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\Plot2d_msg_en.po

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\Plot2d_msg_en.po
InputName=Plot2d_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\Plot2d_msg_en.po
InputName=Plot2d_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_pan.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_pan.png
InputName=plot2d_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_pan.png
InputName=plot2d_pan

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_points.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_points.png
InputName=plot2d_points

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_points.png
InputName=plot2d_points

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_rotate.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_rotate.png
InputName=plot2d_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_rotate.png
InputName=plot2d_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_settings.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_settings.png
InputName=plot2d_settings

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_settings.png
InputName=plot2d_settings

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_splines.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_splines.png
InputName=plot2d_splines

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_splines.png
InputName=plot2d_splines

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\plot2d_zoom.png

!IF  "$(CFG)" == "Plot2d - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_zoom.png
InputName=plot2d_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "Plot2d - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\plot2d_zoom.png
InputName=plot2d_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_FitDataDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_SetupCurveDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_SetupViewDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_ViewFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_ViewModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_Plot2d_ViewWindow.cxx
# End Source File
# End Group
# End Target
# End Project
