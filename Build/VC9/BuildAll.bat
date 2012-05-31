echo off

call "%VS90COMNTOOLS%vsvars32.bat" x86

call ..\..\Config\Tortoise\UpdateVersion.cmd

msbuild AcfAll.sln
msbuild AcfAll.sln /p:Configuration=Release


pause
