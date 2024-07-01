#!/bin/bash

set -e

../bin/buildsuper_x64-linux.sh ../4coder_byp/4coder_byp.cpp $1

# cp ./custom_4coder.* ../../../buildcustom_4coder.*