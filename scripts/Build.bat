IF %compiler%==MinGW GOTO BuildMinGW

:BuildMSVC2013
cmake . -G "Visual Studio 12 2013" -DCMAKE_BUILD_TYPE=%configuration%
call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
msbuild otl.sln /verbosity:minimal /maxcpucount:4 /property:Configuration=%configuration%
GOTO :eof

:BuildMinGW
cmake . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%configuration%
mingw32-make.exe -all
GOTO :eof

:eof