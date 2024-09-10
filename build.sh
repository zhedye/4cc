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
if [ -v custom_layer_only ]; then custom_layer_only=1; echo "[custom layer only]"; fi

# NOTE: mac DEV_BUILD has profiling prints out each frame, which are annoying

# --- Compile/Link Line Definitions -------------------------------------------
os="$(uname -o)" # operating system name
# TODO(edye): is exec_cmd necessary?
# msys = windows git bash
if [ $os == "Msys" ]; then script="build.bat"; custom_layer_script="buildsuper_x64-win.bat"; custom_layer_name="custom_4coder.dll"; fi
if [ $os == "Linux" ];   then script="build-linux.sh"; custom_layer_script="buildsuper_x64-linux.sh"; custom_layer_name="custom_4coder.so";  fi

# Mac OS
if [ $os == "Darwin" ];  then script="build-mac.sh"; custom_layer_script="buildsuper_x64-mac.sh"; custom_layer_name="custom_4coder.so";  fi

if [ ! -v custom_layer_only ]; then
    pushd "code"
    build_cmd=("bin/$script" "$flag")
    "${build_cmd[@]}"
    popd
fi

### --- Custom Layer ----------------------------------------------------------
pushd "code/custom/4coder_edye"

build_custom_cmd=("../bin/$custom_layer_script" "4coder_edye.cpp")
"${build_custom_cmd[@]}"

cp "$custom_layer_name" "../../../build/$custom_layer_name"

# link my bindings
if [ ! -f "../../../build/bindings.4coder" ]; then
ln -s "bindings.4coder" "../../../build/bindings.4coder"
fi

if [ ! -f "../../../build/config.4coder" ]; then
ln -s "config.4coder" "../../../build/config.4coder"    
fi

popd
