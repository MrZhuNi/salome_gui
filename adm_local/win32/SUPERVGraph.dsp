# Microsoft Developer Studio Project File - Name="SUPERVGraph" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SUPERVGraph - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SUPERVGraph.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SUPERVGraph.mak" CFG="SUPERVGraph - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SUPERVGraph - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SUPERVGraph - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SUPERVGraph"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SUPERVGraph - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\SUPERVGraph"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD F90 /browser
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SUPERVGraph_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "NDEBUG" /D "SUPERVGRAPH_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib qwt.lib TKernel.lib TKMath.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\SUPERVGraph.dll" /libpath:"$(QTLIB)" /libpath:"$(QWTLIB)" /libpath:"$(OCCLIB)"

!ELSEIF  "$(CFG)" == "SUPERVGraph - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\SUPERVGraph"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SUPERVGraph_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(OCCINC)" /D "_DEBUG" /D "SUPERVGRAPH_EXPORTS" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TKernel.lib TKMath.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt-mt333.lib qwt.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\SUPERVGraph.dll" /pdbtype:sept /libpath:"$(QTLIBD)" /libpath:"$(QWTLIBD)" /libpath:"$(OCCLIBD)"

!ENDIF 

# Begin Target

# Name "SUPERVGraph - Win32 Release"
# Name "SUPERVGraph - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewModel.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph.h

!IF  "$(CFG)" == "SUPERVGraph - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph.h
InputName=SUPERVGraph

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUPERVGraph - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph.h
InputName=SUPERVGraph

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

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewFrame.h

!IF  "$(CFG)" == "SUPERVGraph - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewFrame.h
InputName=SUPERVGraph_ViewFrame

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUPERVGraph - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewFrame.h
InputName=SUPERVGraph_ViewFrame

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

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewManager.h

!IF  "$(CFG)" == "SUPERVGraph - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewManager.h
InputName=SUPERVGraph_ViewManager

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUPERVGraph - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewManager.h
InputName=SUPERVGraph_ViewManager

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

SOURCE=..\..\src\SUPERVGraph\SUPERVGraph_ViewModel.h

!IF  "$(CFG)" == "SUPERVGraph - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewModel.h
InputName=SUPERVGraph_ViewModel

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SUPERVGraph - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\SUPERVGraph\SUPERVGraph_ViewModel.h
InputName=SUPERVGraph_ViewModel

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
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_SUPERVGraph.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUPERVGraph_ViewFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUPERVGraph_ViewManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_SUPERVGraph_ViewModel.cxx
# End Source File
# End Group
# End Target
# End Project
