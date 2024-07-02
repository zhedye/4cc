@echo off
call ..\bin\buildsuper_x64-win.bat .\4coder_edye.cpp debug 
::release
::move ..\..\..\build\custom_4coder.dll ..\..\..\build\custom_4coder.old.dll
::move ..\..\..\build\custom_4coder.pdb ..\..\..\build\custom_4coder.old.pdb
::copy .\custom_4coder.dll ..\..\..\build\custom_4coder.dll
::copy .\custom_4coder.pdb ..\..\..\build\custom_4coder.pdb