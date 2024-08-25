#!/usr/bin/env sh

set -e

# directories

ME="$(realpath "$0")" # absolute path to script
LOCATION="$(dirname "$ME")" # absolute path to root directory 
SRC_ROOT="$LOCATION/code"
PROJECT_ROOT="$(dirname "$SRC_ROOT")" # relative path to root

if [ ! -d "$PROJECT_ROOT/build" ]; then
mkdir "$PROJECT_ROOT/build"
fi

BUILD_ROOT="$PROJECT_ROOT/build"
BIN_ROOT="$SRC_ROOT/bin"
CUSTOM_ROOT="$SRC_ROOT/custom"
CUSTOM_BIN="$CUSTOM_ROOT/bin"

echo "---"
echo "Building Executable ..."

# DEV_BUILD has profiling prints out each frame, which are annoying
sh "$BIN_ROOT/build-mac.sh" "-DPACKAGE_SUPER_X64" # "-DDEV_BUILD" 

sh "$PROJECT_ROOT/build-custom-mac.sh"