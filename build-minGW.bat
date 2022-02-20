set VS_GENERATOR="MinGW Makefiles"



if not exist "build" mkdir build
cd build
if not exist "minGW" mkdir minGW
cd minGW
cmake -S ../../. -B . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/minGW -DGLFW_INSTALL=OFF -DINSTALL_GTEST=OFF -G %VS_GENERATOR%
mingw32-make
mingw32-make install