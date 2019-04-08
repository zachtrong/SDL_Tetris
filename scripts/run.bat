set buildDir=%~dp0..\build
set scriptDir=%~dp0..\scripts

call %scriptDir%\clean.bat
call %scriptDir%\build.bat
call %buildDir%\Tetris.exe