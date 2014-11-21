@echo off

if [%1]==[MinGW] goto BuildMinGW
if [%1]==[MSVC2013] goto BuildMSVC2013

:BuildMSVC2013
mkdir build
cd build
cmake .. -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%configuration%
if not defined DevEnvDir (
call "%VS120COMNTOOLS%\..\..\VC\vcvarsall.bat"
)
msbuild otl.sln /verbosity:minimal /maxcpucount:4 /property:Configuration=%configuration%
cd ..
goto :eof

:BuildMinGW
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%configuration%
mingw32-make.exe -all
cd ..
goto :eof

:eof