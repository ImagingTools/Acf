copy /Y %QTDIR%\bin\*.dll DebugVC9
copy /Y %QTDIR%\bin\*.dll.Manifest DebugVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* DebugVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* DebugVC9\iconengines\*.*
