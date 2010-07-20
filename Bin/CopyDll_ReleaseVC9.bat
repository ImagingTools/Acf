copy /Y %QTDIR%\bin\*.dll ReleaseVC9
copy /Y %QTDIR%\bin\*.dll.Manifest ReleaseVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ReleaseVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ReleaseVC9\iconengines\*.*
