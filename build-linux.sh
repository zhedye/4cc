#!/usr/bin/env sh

set -e

# directories

ME="$(readlink -f "$0")" # absolute path to script
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
sh "$BIN_ROOT/build-linux.sh" "-DDEV_BUILD" # "-DPACKAGE_SUPER_X64"

sh "$PROJECT_ROOT/build-custom-linux.sh"

