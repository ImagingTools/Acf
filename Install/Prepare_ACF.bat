echo off

echo Preparing ACF framework

copy /Y %QTDIR%\bin\*.dll ..\Bin\ReleaseVC8
copy /Y %QTDIR%\bin\*.dll ..\Bin\DebugVC8
copy /Y ..\Bin\Info\*.xpc ..\Bin\ReleaseVC8
copy /Y ..\Bin\Info\*.xpc ..\Bin\DebugVC8

echo Preparing ACF framework done

