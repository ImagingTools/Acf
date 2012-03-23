echo off

call "%VS90COMNTOOLS%vsvars32.bat" x86

msbuild AcfAll.sln

pause
