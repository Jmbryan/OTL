@echo off

call mex -I"../include" -I"../extlibs/include" OTL_Lambert.cpp ../build/lib/Release/otl-core-s-m-x64.lib
call mex -I"../include" -I"../extlibs/include" OTL_Propagate.cpp ../build/lib/Release/otl-core-s-m-x64.lib
call mex -I"../include" -I"../extlibs/include" OTL_MGADSM.cpp ../build/lib/Release/otl-core-s-m-x64.lib