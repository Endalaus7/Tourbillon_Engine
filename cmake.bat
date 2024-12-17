@echo on

set SDK_DIR=D:/SDK
set CMAKE_DIR=D:/cmake/cmake-3.23.2-windows-i386/cmake-3.23.2-windows-i386/bin

mkdir build
mkdir install

%CMAKE_DIR%/cmake.exe -S . -B build ^
 -DCMAKE_INSTALL_PREFIX=install ^
 -G"Visual Studio 17 2022"

echo cmake.bat build finish!

::%CMAKE_DIR%/cmake.exe --build build --config Release --target install

pause