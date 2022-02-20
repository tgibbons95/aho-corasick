set VS_DEV_CMD_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools"
set MS_BUILD_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin"
set VS_GENERATOR="Visual Studio 16 2019"



call %VS_DEV_CMD_PATH%\VsDevCmd.bat
if not exist "build" mkdir build
cd build
if not exist "msvc" mkdir msvc
cd msvc
cmake -S ../../. -B . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/msvc -DGLFW_INSTALL=OFF -DINSTALL_GTEST=OFF -G %VS_GENERATOR%
%MS_BUILD_PATH%\msbuild.exe INSTALL.vcxproj -t:Build -p:Configuration=Release