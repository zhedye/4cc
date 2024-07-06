#!/usr/bin/env sh

set -e

# directories

ME="$(readlink -f "$0")" # absolute path to script

# absolute path to root directory, /home/user/code/4cc 
LOCATION="$(dirname "$ME")" 
SRC_ROOT="$LOCATION/code"

# relative path to root directory of the project, ./
PROJECT_ROOT="$(dirname "$SRC_ROOT")"

if [ ! -d "$PROJECT_ROOT/build" ]; then
	mkdir "$PROJECT_ROOT/build"
fi

BUILD_ROOT="$PROJECT_ROOT/build"
BIN_ROOT="$SRC_ROOT/bin"
CUSTOM_ROOT="$SRC_ROOT/custom"
CUSTOM_BIN="$CUSTOM_ROOT/bin"

echo "---"
echo "Building Executable ..."
sh "$BIN_ROOT/build-linux.sh" "-DDEV_BUILD" # "-DPACKAGE_SUPER_X64"

sh "$PROJECT_ROOT/build-custom-linux.sh"

