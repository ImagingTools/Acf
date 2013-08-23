echo off

call "%VS90COMNTOOLS%vsvars32.bat" x86

call ..\Tortoise\UpdateVersion.cmd

cd /d %~dp0

msbuild AcfAll.sln /m
msbuild AcfAll.sln /p:Configuration=Release /m

pause
