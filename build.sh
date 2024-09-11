#!/usr/bin/env bash

# -e exit when anything returns non-zero status
# -u unset variables are an error, exit immediately
set -eu

absolute_path="$(dirname $(realpath "$0"))"

# change directory to where this script is
cd "$(dirname "$0")"

# --- Prep Directories --------------------------------------------------------
mkdir -p build

# --- Unpack Arguments --------------------------------------------------------
# -v means a value is set. requires bash 4.2+
for arg in "$@"; do declare $arg='1'; done
if [[  ! -v gcc && ! -v tcc ]]; then clang=1; fi
if [ ! -v super ];      then debug=1; fi
if [ -v debug ];        then echo "[debug dev_build]"; flag="-DDEV_BUILD"; custom_layer_flag="debug"; fi
if [ -v super ];        then echo "[package_super_x64]"; flag="-DPACKAGE_SUPER_X64"; custom_layer_flag="release"; fi
if [ -v custom_layer_only ]; then custom_layer_only=1; echo "[custom layer only]"; fi

# NOTE: mac DEV_BUILD has profiling print outs each frame, which are annoying

# --- Compile/Link Line Definitions -------------------------------------------
os="$(uname -o)" # operating system name
# TODO(edye): is exec_cmd necessary?
# Msys = windows git bash
if [ $os == "Msys" ]; then 
    script="build.bat"; 
    custom_layer_script="buildsuper_x64-win.bat";
    custom_layer_name="custom_4coder.dll"; 
    custom_layer_debug="custom_4coder.pdb"; 
fi

if [ $os == "Linux" ]; then
    script="build-linux.sh"; 
    custom_layer_script="buildsuper_x64-linux.sh"; 
    custom_layer_name="custom_4coder.so"; 
fi

# Mac OS
if [ $os == "Darwin" ]; then
    script="build-mac.sh"; 
    custom_layer_script="buildsuper_x64-mac.sh"; 
    custom_layer_name="custom_4coder.so";  
fi

### --- Build Exectuable ----------------------------------------------------------
if [ ! -v custom_layer_only ]; then
    pushd "code"
    build_cmd=("bin/$script" "$flag")
    "${build_cmd[@]}"
    popd
fi

### --- Build Custom Layer ----------------------------------------------------------
pushd "code/custom/4coder_edye"

build_custom_cmd=("../bin/$custom_layer_script" "4coder_edye.cpp" "$custom_layer_flag")
# "${build_custom_cmd[@]}"

cp "$custom_layer_name" "../../../build/$custom_layer_name"

if [ -v custom_layer_debug ]; then
    cp "$custom_layer_debug" "../../../build/$custom_layer_debug"
fi

popd

// TODO(edye): change lexer for bash script. auto indent if statements
# link my bindings
if [[ ! -e "../../../build/bindings.4coder" ]]; then
    ln -s "$absolute_path/code/custom/4coder_edye/bindings.4coder" "build/bindings.4coder"
fi

if [[ ! -e "../../../build/config.4coder" ]]; then
    ln -s "$absolute_path/code/custom/4coder_edye/config.4coder" "build/config.4coder"
fi


# --- Fonts 

cp -r "non-source/dist_files/fonts/" "build/"
