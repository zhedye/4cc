# Personal 4coder Custom Layer

## Build:

build.bat

build-linux.sh

build-mac.sh


# symbolic linking config

## Windows 

open up command prompt as administrator:

mklink C:\Users\hahah\code\4cc\build\config.4coder C:\Users\hahah\code\4cc\code\custom\4coder_edye\config.4coder

mklink C:\Users\hahah\code\4cc\build\bindings.4coder C:\Users\hahah\code\4cc\code\custom\4coder_edye\bindings.4coder


## Linux and Mac

ln -s ~/code/4cc/code/custom/4coder_edye/bindings.4coder ~/code/4cc/build/bindings.4coder && ln -s ~/code/4cc/code/custom/4coder_edye/config.4coder ~/code/4cc/build/config.4coder