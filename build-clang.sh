#!/bin/sh

export CC=/usr/bin/clang 
export CXX=/usr/bin/clang++ 
mkdir -p build/clang 
cd build/clang
cmake -S ../../. -B . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/clang -DGLFW_INSTALL=OFF -DINSTALL_GTEST=OFF
make all
make install
