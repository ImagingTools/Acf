rem echo off

set COMPILER_EXT=VC12
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2013

cd ..\..

echo Generating %COMPILER_EXT% projects...
cd Build/QMake
%QTDIR%\bin\qmake -recursive -tp vc AcfAll.pro
cd ../..

call %ACFDIR%/Config/QMake/CopyVCProjToSubdir.js %COMPILER_EXT%


cd Build\%COMPILER_EXT%
