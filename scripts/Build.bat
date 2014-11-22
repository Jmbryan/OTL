@echo off

if exist build\ rmdir build\ /s /q
mkdir build
cd build

if [%1]==[MSVC2013] goto BuildMSVC2013
if [%1]==[MinGW] goto BuildMinGW
goto End

:BuildMSVC2013
cmake .. -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%2 -DCMAKE_INSTALL_PREFIX="./install"
if not defined DevEnvDir (
call "%VS120COMNTOOLS%\..\..\VC\vcvarsall.bat"
)
msbuild otl.sln /verbosity:minimal /maxcpucount:4 /property:Configuration=%2
goto End

:BuildMinGW
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%2
mingw32-make.exe -all
goto End

:End
cd ..

:eof