#!/bin/bash

git clone --recurse-submodules https://github.com/arvidn/libtorrent.git
cd libtorrent
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=14 -G Ninja ..
ninja -j4

cd ..
sudo cp -rf include/libtorrent   /usr/include
sudo cp -rf build /usr/lib

