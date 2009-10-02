echo off

echo Building ACF

cd ..\Impl\%COMPILER_EXT%
vcbuild /r AcfAll.sln $All

echo Building ACF done

echo Building ACF Tutorials

cd ..\..\Docs\Tutorial\Appl\%COMPILER_EXT%
vcbuild /r TutorialAcf.sln $All
echo Building ACF Tutorials done

cd ..\..\..\..\Install

