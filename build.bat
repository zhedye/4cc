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

cd %location%

set mode=%1
if "%mode%" == "" (set mode="/DDEV_BUILD")

echo "---"
echo "Building Executable ..."
pushd %src_root%
call %bin_root%\build.bat %mode%
popd

call build-custom.bat