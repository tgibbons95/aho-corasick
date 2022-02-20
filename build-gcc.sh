#!/bin/sh

export CC=/usr/bin/gcc 
export CXX=/usr/bin/g++ 
mkdir -p build/gcc 
cd build/gcc
cmake -S ../../. -B . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/gcc -DGLFW_INSTALL=OFF -DINSTALL_GTEST=OFF
make all
make install
