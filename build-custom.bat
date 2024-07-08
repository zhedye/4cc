@echo off

set location=%cd%
set me="%~dp0"

REM 4cc\code\bin
cd %me%

REM 4cc
set repo_root=%cd%
set src_root=%repo_root%\code
set build_root=%repo_root%\build
set bin_root=%repo_root%\code\bin
set custom_root=%repo_root%\code\custom
set custom_bin=%custom_root\bin

echo ""
echo "---"
echo "Building custom layer ... "
pushd %custom_root%\4coder_edye
call ..\bin\buildsuper_x64-win.bat 4coder_edye.cpp
popd

rem Copy custom_4coder.so to BUILD_ROOT, overwriting if it already exists (/Y flag).
xcopy /Y "%custom_root%\4coder_edye\custom_4coder.dll" "%build_root%\custom_4coder.dll"
xcopy /Y "%custom_root%\4coder_edye\custom_4coder.pdb" "%build_root%\custom_4coder.pdb"

rem Check if bindings.4coder already exists in BUILD_ROOT, create symbolic link if not
if not exist "%build_root%\bindings.4coder" (
    mklink "%build_root%\bindings.4coder" "%custom_root%\4coder_edye\bindings.4coder"
)

rem Check if config.4coder already exists in BUILD_ROOT, create symbolic link if not
if not exist "%build_root%\config.4coder" (
    mklink "%build_root%\config.4coder" "%custom_root%\4coder_edye\config.4coder"
)   