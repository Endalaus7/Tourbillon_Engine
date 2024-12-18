@echo on

set SDK_DIR=./3rdParty
set CMAKE_DIR=D:/CMake/bin

mkdir build
mkdir install

%CMAKE_DIR%/cmake.exe -S . -B build ^
 -DCMAKE_INSTALL_PREFIX=install ^
 -G"Visual Studio 17 2022"

echo cmake.bat build finish!

::%CMAKE_DIR%/cmake.exe --build build --config Release --target install

pause