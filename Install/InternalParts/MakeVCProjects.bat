echo Generating %COMPILER_EXT% projects...
cd Build/QMake
qmake -recursive -tp vc AcfAll.pro
cd ../..

call %ACFDIR%/Config/QMake/CopyVCProjToSubdir.js %COMPILER_EXT%

