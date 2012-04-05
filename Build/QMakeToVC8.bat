echo Generating VC8 projects...

cd ..
call Config\QMake\CopyProToSubdir.js VC8
cd Build\VC8

set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2005
qmake -recursive -tp vc AcfAll.pro

del /q Static.sln
del /q Component.sln
del /q Tool.sln

cd ..\..
call Config\QMake\RemoveProFromSubdir.js VC8
cd Build

pause
