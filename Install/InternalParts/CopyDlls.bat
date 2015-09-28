echo off

echo Preparing ACF framework

xcopy /Y %QTDIR%\bin\icu*.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Cored.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Guid.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Widgetsd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Xmld.dll Bin\DebugVC12\

xcopy /Y %QTDIR%\bin\icu*.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Core.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Gui.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Widgets.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Xml.dll Bin\ReleaseVC12\

echo Preparing ACF framework done

