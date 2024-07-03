#!/usr/bin/env sh

sh code/bin/build-linux.sh "-DDEV_BUILD" # "-DPACKAGE_SUPER_X64"

pushd code/custom/4coder_edye
sh build-linux.sh
popd