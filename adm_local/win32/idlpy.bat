@set PYTHONPATH=%OMNILIB%\..\python;%PYTHONPATH%
@%OMNIROOT%\bin\x86_win32\omniidl.exe -p%OMNIORBPYROOT%\lib\python -bpython -I../../idl -I%KERNEL_ROOT_DIR%/idl/ -C../../win32/python %1
