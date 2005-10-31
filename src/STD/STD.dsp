# Microsoft Developer Studio Project File - Name="STD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=STD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "STD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "STD.mak" CFG="STD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "STD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "STD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "STD"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "STD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\STD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STD_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /D "STD_EXPORTS" /D "WNT" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib qt-mt333.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\STD.dll" /libpath:"$(QTLIB)"

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\STD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STD_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /D "STD_EXPORTS" /D "WNT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib qt-mt333.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\STD.dll" /pdbtype:sept /libpath:"$(QTLIBD)"

!ENDIF 

# Begin Target

# Name "STD - Win32 Release"
# Name "STD - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\STD_Application.cxx
# End Source File
# Begin Source File

SOURCE=.\STD_CloseDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\STD_LoadStudiesDlg.cxx
# End Source File
# Begin Source File

SOURCE=.\STD_MDIDesktop.cxx
# End Source File
# Begin Source File

SOURCE=.\STD_SDIDesktop.cxx
# End Source File
# Begin Source File

SOURCE=.\STD_TabDesktop.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\STD.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\STD.h
InputName=STD

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\STD.h
InputName=STD

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\STD_Application.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_Application.h
InputName=STD_Application

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_Application.h
InputName=STD_Application

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

SOURCE=.\STD_CloseDlg.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_CloseDlg.h
InputName=STD_CloseDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_CloseDlg.h
InputName=STD_CloseDlg

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

SOURCE=.\STD_LoadStudiesDlg.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_LoadStudiesDlg.h
InputName=STD_LoadStudiesDlg

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_LoadStudiesDlg.h
InputName=STD_LoadStudiesDlg

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

SOURCE=.\STD_MDIDesktop.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_MDIDesktop.h
InputName=STD_MDIDesktop

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_MDIDesktop.h
InputName=STD_MDIDesktop

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

SOURCE=.\STD_SDIDesktop.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_SDIDesktop.h
InputName=STD_SDIDesktop

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_SDIDesktop.h
InputName=STD_SDIDesktop

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

SOURCE=.\STD_TabDesktop.h

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_TabDesktop.h
InputName=STD_TabDesktop

BuildCmds= \
	type $(InputPath) > ..\..\include\$(InputName).h \
	$(QTDIR)\bin\moc.exe $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\STD_TabDesktop.h
InputName=STD_TabDesktop

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

SOURCE=.\resources\close.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\close.png
InputName=close

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\close.png
InputName=close

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\config

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\config
InputName=config

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\config
InputName=config

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\copy.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\copy.png
InputName=copy

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\copy.png
InputName=copy

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\cursor_rotate.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cursor_rotate.png
InputName=cursor_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cursor_rotate.png
InputName=cursor_rotate

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\cursor_zoom.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cursor_zoom.png
InputName=cursor_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cursor_zoom.png
InputName=cursor_zoom

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\cut.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cut.png
InputName=cut

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\cut.png
InputName=cut

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\help.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\help.png
InputName=help

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\help.png
InputName=help

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\new.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\new.png
InputName=new

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\new.png
InputName=new

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\open.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\open.png
InputName=open

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\open.png
InputName=open

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\paste.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\paste.png
InputName=paste

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\paste.png
InputName=paste

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\print.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\print.png
InputName=print

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\print.png
InputName=print

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\redo.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\redo.png
InputName=redo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\redo.png
InputName=redo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\reset.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\reset.png
InputName=reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\reset.png
InputName=reset

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\save.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\save.png
InputName=save

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\save.png
InputName=save

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\std.ini

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).ini...
InputPath=.\resources\std.ini
InputName=std

"..\..\resources\$(InputName).ini" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).ini

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).ini...
InputPath=.\resources\std.ini
InputName=std

"..\..\resources\$(InputName).ini" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).ini

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\STD_images.po

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\STD_images.po
InputName=STD_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\STD_images.po
InputName=STD_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\STD_msg_en.po

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\STD_msg_en.po
InputName=STD_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\STD_msg_en.po
InputName=STD_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\undo.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\undo.png
InputName=undo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\undo.png
InputName=undo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources\undo_arrow.png

!IF  "$(CFG)" == "STD - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\undo_arrow.png
InputName=undo_arrow

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "STD - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\undo_arrow.png
InputName=undo_arrow

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_STD_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_STD_CloseDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_STD_LoadStudiesDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_STD_MDIDesktop.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_STD_SDIDesktop.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_STD_TabDesktop.cxx
# End Source File
# End Group
# End Target
# End Project
