# Microsoft Developer Studio Project File - Name="LightApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=LightApp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LightApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LightApp.mak" CFG="LightApp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LightApp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "LightApp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "LightApp"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LightApp - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\win32\lib"
# PROP Intermediate_Dir "..\..\win32\obj\LightApp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD F90 /browser
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIGHTAPP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include" /I "$(QTDIR)\include" /I "$(QWTINC)" /I "$(PYTHONINC)" /I "$(OCCINC)" /I "$(VTKINC)" /D "LIGHTAPP_EXPORTS" /D "NDEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 qt-mt333.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib TKernel.lib TKV3d.lib vtkCommon.lib vtkRendering.lib /nologo /dll /machine:I386 /out:"..\..\win32\bin\LightApp.dll" /libpath:"$(OCCLIB)" /libpath:"$(QTLIB)" /libpath:"$(PYTHONLIB)" /libpath:"$(VTKLIB)"

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\win32\libd"
# PROP Intermediate_Dir "..\..\win32\objd\LightApp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIGHTAPP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include" /I "$(QTDIR)\include" /I "$(QWTINC)" /I "$(PYTHONINC)" /I "$(OCCINC)" /I "$(VTKINC)" /D "LIGHTAPP_EXPORTS" /D "_DEBUG" /D "WNT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 qt-mt333.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib TKernel.lib TKV3d.lib vtkCommon.lib vtkRendering.lib /nologo /dll /debug /machine:I386 /out:"..\..\win32\bind\LightApp.dll" /pdbtype:sept /libpath:"$(OCCLIBD)" /libpath:"$(QTLIBD)" /libpath:"$(PYTHONLIB)" /libpath:"$(VTKLIB)"

!ENDIF 

# Begin Target

# Name "LightApp - Win32 Release"
# Name "LightApp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_AboutDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataOwner.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataSubOwner.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Dialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Displayer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Driver.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_GLSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Module.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_ModuleDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_NameDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_OBFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_OBSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_OCCSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Operation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Preferences.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_PreferencesDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Selection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_SelectionMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_ShowHideOp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Study.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_SwitchOp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_VTKSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_WidgetContainer.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp.h
InputName=LightApp

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp.h
InputName=LightApp

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_AboutDlg.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_AboutDlg.h
InputName=LightApp_AboutDlg

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_AboutDlg.h
InputName=LightApp_AboutDlg

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

SOURCE=..\..\src\LightApp\LightApp_Application.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Application.h
InputName=LightApp_Application

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Application.h
InputName=LightApp_Application

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

SOURCE=..\..\src\LightApp\LightApp_DataModel.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_DataModel.h
InputName=LightApp_DataModel

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_DataModel.h
InputName=LightApp_DataModel

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

SOURCE=..\..\src\LightApp\LightApp_DataObject.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataObject.h
InputName=LightApp_DataObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataObject.h
InputName=LightApp_DataObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataOwner.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataOwner.h
InputName=LightApp_DataOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataOwner.h
InputName=LightApp_DataOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_DataSubOwner.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataSubOwner.h
InputName=LightApp_DataSubOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_DataSubOwner.h
InputName=LightApp_DataSubOwner

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Dialog.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Dialog.h
InputName=LightApp_Dialog

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Dialog.h
InputName=LightApp_Dialog

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

SOURCE=..\..\src\LightApp\LightApp_Displayer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Driver.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Driver.h
InputName=LightApp_Driver

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Driver.h
InputName=LightApp_Driver

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

SOURCE=..\..\src\LightApp\LightApp_GLSelector.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_GLSelector.h
InputName=LightApp_GLSelector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_GLSelector.h
InputName=LightApp_GLSelector

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

SOURCE=..\..\src\LightApp\LightApp_Module.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Module.h
InputName=LightApp_Module

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Module.h
InputName=LightApp_Module

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

SOURCE=..\..\src\LightApp\LightApp_ModuleDlg.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_ModuleDlg.h
InputName=LightApp_ModuleDlg

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_ModuleDlg.h
InputName=LightApp_ModuleDlg

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

SOURCE=..\..\src\LightApp\LightApp_NameDlg.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_NameDlg.h
InputName=LightApp_NameDlg

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_NameDlg.h
InputName=LightApp_NameDlg

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

SOURCE=..\..\src\LightApp\LightApp_OBFilter.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_OBFilter.h
InputName=LightApp_OBFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_OBFilter.h
InputName=LightApp_OBFilter

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_OBSelector.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_OBSelector.h
InputName=LightApp_OBSelector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_OBSelector.h
InputName=LightApp_OBSelector

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

SOURCE=..\..\src\LightApp\LightApp_OCCSelector.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_OCCSelector.h
InputName=LightApp_OCCSelector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_OCCSelector.h
InputName=LightApp_OCCSelector

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

SOURCE=..\..\src\LightApp\LightApp_Operation.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Operation.h
InputName=LightApp_Operation

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Operation.h
InputName=LightApp_Operation

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

SOURCE=..\..\src\LightApp\LightApp_Preferences.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Preferences.h
InputName=LightApp_Preferences

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Preferences.h
InputName=LightApp_Preferences

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

SOURCE=..\..\src\LightApp\LightApp_PreferencesDlg.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_PreferencesDlg.h
InputName=LightApp_PreferencesDlg

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_PreferencesDlg.h
InputName=LightApp_PreferencesDlg

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

SOURCE=..\..\src\LightApp\LightApp_RootObject.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_RootObject.h
InputName=LightApp_RootObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_RootObject.h
InputName=LightApp_RootObject

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_Selection.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_Selection.h
InputName=LightApp_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_Selection.h
InputName=LightApp_Selection

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_SelectionMgr.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_SelectionMgr.h
InputName=LightApp_SelectionMgr

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_SelectionMgr.h
InputName=LightApp_SelectionMgr

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

SOURCE=..\..\src\LightApp\LightApp_ShowHideOp.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build
InputPath=.\LightApp_ShowHideOp.h
InputName=LightApp_ShowHideOp

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build
InputPath=.\LightApp_ShowHideOp.h
InputName=LightApp_ShowHideOp

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

SOURCE=..\..\src\LightApp\LightApp_Study.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Study.h
InputName=LightApp_Study

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_Study.h
InputName=LightApp_Study

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

SOURCE=..\..\src\LightApp\LightApp_SwitchOp.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_SwitchOp.h
InputName=LightApp_SwitchOp

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_SwitchOp.h
InputName=LightApp_SwitchOp

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

SOURCE=..\..\src\LightApp\LightApp_UpdateFlags.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_UpdateFlags.h
InputName=LightApp_UpdateFlags

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).h...
InputPath=.\LightApp_UpdateFlags.h
InputName=LightApp_UpdateFlags

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\include\$(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\LightApp_VTKSelector.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_VTKSelector.h
InputName=LightApp_VTKSelector

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_VTKSelector.h
InputName=LightApp_VTKSelector

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

SOURCE=..\..\src\LightApp\LightApp_WidgetContainer.h

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_WidgetContainer.h
InputName=LightApp_WidgetContainer

BuildCmds= \
	$(QTDIR)\bin\moc $(InputPath) -o ..\..\moc\moc_$(InputName).cxx \
	type $(InputPath) > ..\..\include\$(InputName).h \
	

"..\..\moc\moc_$(InputName).cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate moc_$(InputName).cxx...
InputPath=.\LightApp_WidgetContainer.h
InputName=LightApp_WidgetContainer

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
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_about.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_about.png
InputName=icon_about

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_about.png
InputName=icon_about

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_applogo.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_applogo.png
InputName=icon_applogo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_applogo.png
InputName=icon_applogo

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_default.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_default.png
InputName=icon_default

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_default.png
InputName=icon_default

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_module.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_module.png
InputName=icon_module

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_module.png
InputName=icon_module

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_module_big.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_module_big.png
InputName=icon_module_big

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_module_big.png
InputName=icon_module_big

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\icon_select.png

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_select.png
InputName=icon_select

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).png...
InputPath=.\resources\icon_select.png
InputName=icon_select

"..\..\resources\$(InputName).png" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).png

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\LightApp.ini

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).ini...
InputPath=.\resources\LightApp.ini
InputName=LightApp

"..\..\resources\$(InputName).ini" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).ini

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).ini...
InputPath=.\resources\LightApp.ini
InputName=LightApp

"..\..\resources\$(InputName).ini" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).ini

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\LightApp.xml

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Update $(InputName).xml...
InputPath=.\resources\LightApp.xml
InputName=LightApp

"..\..\resources\$(InputName).xml" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).xml

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Update $(InputName).xml...
InputPath=.\resources\LightApp.xml
InputName=LightApp

"..\..\resources\$(InputName).xml" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	type $(InputPath) > ..\..\resources\$(InputName).xml

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\LightApp_images.po

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\LightApp_images.po
InputName=LightApp_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\LightApp_images.po
InputName=LightApp_images

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\LightApp\resources\LightApp_msg_en.po

!IF  "$(CFG)" == "LightApp - Win32 Release"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\LightApp_msg_en.po
InputName=LightApp_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ELSEIF  "$(CFG)" == "LightApp - Win32 Debug"

# Begin Custom Build - Generate $(InputName).qm...
InputPath=.\resources\LightApp_msg_en.po
InputName=LightApp_msg_en

"..\..\resources\$(InputName).qm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\msg2qm.exe $(InputPath) ..\..\resources\$(InputName).qm

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_AboutDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_DataModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Dialog.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Driver.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_GLSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Module.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_ModuleDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_NameDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_OBSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_OCCSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Operation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Preferences.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_PreferencesDlg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_SelectionMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_ShowHideOp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_Study.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_SwitchOp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_VTKSelector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\moc\moc_LightApp_WidgetContainer.cxx
# End Source File
# End Group
# End Target
# End Project
