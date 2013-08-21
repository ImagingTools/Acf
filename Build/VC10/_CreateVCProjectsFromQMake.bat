rem echo off

set COMPILER_EXT=VC10
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2010

cd ..\..

echo Generating %COMPILER_EXT% projects...
cd Build/QMake
qmake -recursive -tp vc AcfAll.pro
cd ../..

call %ACFDIR%/Config/QMake/CopyVCProjToSubdir.js %COMPILER_EXT%


cd Build\VC10
