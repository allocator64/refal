@Echo off
set org=%1.
set t=%1.
:NC
for %%a in (/%org%) do set t=%%a
for %%a in (/%org%) do if %%a%t%==%org% set c=%%a
set org=%t%
if %c%==. goto P
set src=%src%%c%
if %t%==. goto Finish
goto NC
:P
set word=%c%
:NC1
for %%a in (/%org%) do set t=%%a
if %t%==. goto Finish
for %%a in (/%org%) do if %%a%t%==%org% set c=%%a
set org=%t%
set word=%word%%c%
if not %c%==\ goto NC1
set src=%src%%word%
set word=
goto NC
:Finish
set org=
set t=
set c=
t:\refal\refal %1
t:\refal\reflink t:\Refal\mainrf.obj %src%.obj t:\REFAL\c0l.obj,%src%.exe,NUL,t:\Refal\reflib.lib t:\REFAL\cl.lib
erase %src%.obj
erase %src%.lst
%src%.exe
set src=