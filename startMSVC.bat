@echo off

set opt=d
if %1.==r. set opt=

call env.bat %opt%

rem set
msdev %SUITROOT%\SUIT\SUIT.dsw
