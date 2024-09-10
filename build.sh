#!/usr/bin/env bash

# -e exit when anything returns non-zero status
# -u unset variables are an error, exit immediately
set -eu

# change directory to where this script is
cd "$(dirname "$0")"

# --- Prep Directories --------------------------------------------------------
mkdir -p build

# --- Unpack Arguments --------------------------------------------------------
# -v means a value is set. requires bash 4.2+
for arg in "$@"; do declare $arg='1'; done
if [[  ! -v gcc && ! -v tcc ]]; then clang=1; fi
if [ ! -v super ];      then debug=1; fi
if [ -v debug ];        then echo "[dev_build]"; flag="-DDEV_BUILD"; fi
if [ -v super ];        then echo "[package_super_x64]"; flag="-DPACKAGE_SUPER_X64"; fi
if [ -v custom_layer ]; then customer_layer=1; echo "[custom layer only]"; fi

# NOTE: mac DEV_BUILD has profiling prints out each frame, which are annoying

# --- Compile/Link Line Definitions -------------------------------------------
os="$(uname -s)" # operating system name
# TODO do i even need exec_cmd? idk. test without it on windows git bash
if [ $os == "Windows" ]; then exec_cmd="call"; script="build.bat"; custom_layer_script="buildsuper_x64-win.bat"; fi
if [ $os == "Linux" ];   then exec_cmd="bash"; script="build-linux.sh"; custom_layer_script="buildsuper_x64-linux.sh"; fi

# Mac OS
if [ $os == "Darwin" ];  then exec_cmd="bash"; script="build-mac.sh"; custom_layer_script="buildsuper_x64-mac.sh"; fi

if [ ! -v custom_layer ]; then
build_cmd=("$exec_cmd" "code/bin/$script" "$flag")
"${build_cmd[@]}"
fi

pushd "code/custom/4coder_edye"

build_custom_cmd=("$exec_cmd" "../bin/$custom_layer_script" "4coder_edye.cpp")
"${build_custom_cmd[@]}"

cp "custom_4coder.so" "../../../build/custom_4coder.so"

# link my bindings
if [ ! -f "../../../build/bindings.4coder" ]; then
ln -s "bindings.4coder" "../../../build/bindings.4coder"
fi

if [ ! -f "../../../build/config.4coder" ]; then
ln -s "config.4coder" "../../../build/config.4coder"    
fi

popd
