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

echo ""
echo "---"
echo "Building custom layer ... "
pushd "$CUSTOM_ROOT/4coder_edye"
../bin/buildsuper_x64-mac.sh 4coder_edye.cpp
popd

cp "$CUSTOM_ROOT/4coder_edye/custom_4coder.so" "$BUILD_ROOT/custom_4coder.so"

if [ ! -f "$BUILD_ROOT/bindings.4coder" ]; then
ln -s "$CUSTOM_ROOT/4coder_edye/bindings.4coder" "$BUILD_ROOT/bindings.4coder"
fi

if [ ! -f "$BUILD_ROOT/config.4coder" ]; then
ln -s "$CUSTOM_ROOT/4coder_edye/config.4coder" "$BUILD_ROOT/config.4coder"    
fi
