copy /Y %QTDIR%\bin\*.dll ReleaseVC8
copy /Y %QTDIR%\bin\*.dll.Manifest ReleaseVC8
xcopy /Y %QTDIR%\plugins\imageformats\*.* ReleaseVC8\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ReleaseVC8\iconengines\*.*
