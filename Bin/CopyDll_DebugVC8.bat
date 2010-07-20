copy /Y %QTDIR%\bin\*.dll DebugVC8
copy /Y %QTDIR%\bin\*.dll.Manifest DebugVC8
xcopy /Y %QTDIR%\plugins\imageformats\*.* DebugVC8\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* DebugVC8\iconengines\*.*
