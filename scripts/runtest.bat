@echo off

set testDir=%~dp0..\build_test\
set scriptDir=%~dp0..\scripts

call %scriptDir%\clean.bat
call %scriptDir%\test.bat
call %testDir%\test.exe