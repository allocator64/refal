@echo off
if .%1 == . goto USAGE
if exist    %2\lib\cl.lib goto BEGIN
if exist  %TC%\lib\cl.lib %0 %1 %TC%
if exist %R2K%\lib\cl.lib %0 %1 %R2K%
echo.
echo ERROR:  Turbo C library not found.
goto USAGE~
:BEGIN
..\refal test%1 (ns)
%2\tlink ..\rfdbg test%1 %2\LIB\c0l,test%1,NUL,..\reflib ..\rfdbg %2\LIB\cl
test%1 < debug.txt 
erase test%1.obj
erase test%1.exe
goto END
:USAGE
echo.
echo EXECUTE TEST FOR REFAL SYSTEM
:USAGE~
echo.
echo         Usage:    TEST Test-number Turbo-C-directory
echo         Example:  TEST 1 C:\TC
echo.
echo It is a good idea to have in AUTOEXEC.BAT file a command
echo                   SET TC=Turbo-C-directory
echo and to have Turbo C object libraries in its \LIB subdirectory.
echo In this case you can omit the second argument, for example:
echo.
echo In AUTOEXEC.BAT:  SET TC=C:\TC
echo This file usage:  TEST 1
:END
