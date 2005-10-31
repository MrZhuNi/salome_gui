@echo off

if exist custom.bat call custom.bat %1

if %SUITROOT%.==. set SUITROOT=.
set SUITRoot=%SUITROOT%
if %CSF_stdResources%.==. set CSF_stdResources=%SUITROOT%\resources

if %CASROOT%.==. set CASROOT=\\aquarex\nnt_03\Common\Occ521\ros
if %OCCINC%.==. set OCCINC=%CASROOT%\inc
if %OCCLIB%.==. set OCCLIB=%CASROOT%\win32\lib
if %OCCBIN%.==. set OCCBIN=%CASROOT%\win32\bin
if %OCCLIBD%.==. set OCCLIBD=\\aquarex\nnt_03\OS\OCC\rosmntdeb\wnt\lib
if not exist %OCCLIBD% set OCCLIBD=%OCCLIB%
if %OCCBIND%.==. set OCCBIND=%OCCLIBD%
if not exist %OCCBIND% set OCCBIND=%OCCBIN%

if %OCCFIX%.==. set OCCFIX=\\aquarex\nnt_03\Principia\OCCFIX
if %OCCFIXINC%.==. set OCCFIXINC=%OCCFIX%\inc
if %OCCFIXLIB%.==. set OCCFIXLIB=%OCCFIX%\win32\lib
if %OCCFIXBIN%.==. set OCCFIXBIN=%OCCFIX%\win32\bin
if %OCCFIXLIBD%.==. set OCCFIXLIBD=%OCCFIX%\win32\libd
if %OCCFIXBIND%.==. set OCCFIXBIND=%OCCFIX%\win32\bind

if %QTDIR%.==. set QTDIR=\\aquarex\nnt_03\PRODUCTS\qt333

if %PYTHONHOME%.==. set PYTHONHOME=\\aquarex\nnt_03\PRODUCTS\Python22

if %VTKHOME%.==. set VTKHOME=E:\PRODUCTS\VTK42 
if %QWTHOME%.==. set QWTHOME=W:\Principia\tmp\qwt-0.4.1 

if %1.==d. set PATH=%OCCFIXBIND%;%OCCBIND%;%QTDIR%\bin;%QWTLIBD%;%PYTHONHOME%;%VTKBIN%;%PATH%
if not %1.==d. set PATH=%OCCFIXBIN%;%OCCBIN%;%QTDIR%\bin;%QWTLIB%;%PYTHONHOME%;%VTKBIN%;%PATH%

